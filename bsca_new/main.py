from argparse import ArgumentParser
from hashlib import md5
from json import dump, load
from os import environ
from pathlib import Path
from re import sub
from typing import TypedDict

import numpy as np
from faiss import IndexFlatIP, read_index, write_index
from openai import OpenAI
from tree_sitter import Language, Parser, Query, QueryCursor
from tree_sitter_c import language

client = OpenAI()
EMBEDDING_MODEL = environ["OPENAI_DEFAULT_MODEL"]


class Function(TypedDict):
    path: str
    declaration: str
    content: str
    index: int


def split_window(content: str, window_size: int = 1000):
    lines = content.splitlines()
    windows: list[str] = []
    curr_window: list[str] = []
    curr_window_len = 0
    for line in lines:
        curr_window.append(line)
        curr_window_len += len(line.strip())
        if curr_window_len > window_size:
            windows.append("\n".join(curr_window))
            curr_window = []
            curr_window_len = 0
    if curr_window:
        windows.append("\n".join(curr_window))
    return windows


def chunk_files(repo_path: Path, min_func_len: int = 10) -> list[Function]:
    c_lang = Language(language())
    parser = Parser(c_lang)
    query = Query(c_lang, "(function_definition declarator: (_)@decl)@fnc_def")
    cur = QueryCursor(query)
    funcs: list[Function] = []
    for file in repo_path.rglob("*.c"):
        file_content = file.read_bytes()
        tree = parser.parse(file_content)
        for _, caps in cur.matches(tree.root_node):
            # Get all functions
            func = caps["fnc_def"][0]
            if func.end_point[0] - func.start_point[0] < min_func_len:
                # Filter small function
                continue
            decl = caps["decl"][0]
            declaration = file_content[decl.start_byte : decl.end_byte].decode()
            # Normalize function name
            # TODO: Should we normalize other variable?
            func_name = declaration.split("(")[0]
            content = (
                file_content[func.start_byte : func.end_byte]
                .decode()
                .replace(func_name, "sub_0")
            )
            windows = split_window(content)
            for idx, window in enumerate(windows):
                # Split to small window for better query
                funcs.append(
                    {
                        "path": str(file),
                        "declaration": declaration,
                        "content": window,
                        "index": idx,
                    }
                )
    return funcs


def embedding(texts: list[str], batch_size: int = 256):
    vecs = []
    for start in range(0, len(texts), batch_size):
        batch = texts[start : start + batch_size]
        resp = client.embeddings.create(model=EMBEDDING_MODEL, input=batch)
        vecs.extend(item.embedding for item in sorted(resp.data, key=lambda d: d.index))
        print(f"  embedded {min(start + batch_size, len(texts))}/{len(texts)}")
    arr = np.asarray(vecs, dtype="float32")
    norms = np.linalg.norm(arr, axis=1, keepdims=True)
    norms[norms == 0] = 1.0
    return arr / norms


def winnow(text: str, k: int = 5, window: int = 4):
    text = "".join(text.lower().split())
    kgrams = [text[i : i + k] for i in range(len(text) - k + 1)]
    hashes = [int(md5(g.encode()).hexdigest(), 16) for g in kgrams]
    fingerprints: dict[int, int] = {}
    last_pos = -1
    # Slide a window over the hashes
    for start in range(len(hashes) - window + 1):
        win = hashes[start : start + window]
        # Select the minimum in each window
        smallest = min(win)
        pos = start + len(win) - 1 - win[::-1].index(smallest)  # rightmost minimum
        if pos != last_pos:
            # Record when position changed
            fingerprints[pos] = smallest
            last_pos = pos
    return set(fingerprints.values())


def canonicalize(text: str) -> str:
    """
    Canonicalize input for winnowing, canonicalize before embedding will cause downgrade
    """
    text = sub(r"//[^\n]*", "", text)  # comments
    text = sub(r"sub_[0-9A-Fa-f]+", "sub_X", text)  # function/callee names
    text = sub(r"\b[a-z]\d+\b", "var", text)  # v9, a1, v2, ...
    text = sub(r"__\w+", "", text)  # __fastcall, __int64, ...
    return text


def embedding_preprocess(text: str):
    """
    Strip comment and remove whitespace
    """
    text = sub(r"//[^\n]*", "", text)
    return " ".join(text.split())


def jaccard(a: set[int], b: set[int]) -> float:
    union = a | b
    return len(a & b) / len(union) if union else 0.0


def index(repo_path: Path, index_folder: Path):
    funcs = chunk_files(repo_path)
    vecs = embedding([embedding_preprocess(i["content"]) for i in funcs])
    index = IndexFlatIP(vecs.shape[1])  # inner product on normalized vectors = cosine
    index.add(vecs)
    write_index(index, str(index_folder / "index.faiss"))
    with open(index_folder / "chunks.json", "w") as f:
        dump(funcs, f)


def search(
    query: Path | str,
    index_folder: Path,
    top_k: int,
    lexical_weight: float = 3.0,
    candidate_factor: int = 10,
):
    # Read query
    if isinstance(query, str):
        query_text = query
        # Add prefix for better query result
        query_embed_text = "Query: " + query_text
    else:
        # Read code file directly
        query_text = query.read_text()
        query_embed_text = query_text
    # Generate vector and search
    query_vec = embedding([embedding_preprocess(query_embed_text)])
    index = read_index(str(index_folder / "index.faiss"))
    with open(index_folder / "chunks.json") as f:
        funcs: list[Function] = load(f)
    n_candidates = min(len(funcs), max(top_k * candidate_factor, top_k))
    scores, ids = index.search(query_vec, n_candidates)
    # Rerank candidates by combining cosine similarity with winnowing
    query_fp = winnow(canonicalize(query_text))
    reranked: list[tuple[float, float, float, int]] = []
    for score, i in zip(scores[0], ids[0]):
        i = int(i)
        if i < 0:
            continue
        lexical = jaccard(query_fp, winnow(canonicalize(funcs[i]["content"])))
        combined = float(score) + lexical_weight * lexical
        reranked.append((combined, float(score), lexical, i))
    reranked.sort(key=lambda item: item[0], reverse=True)
    for rank, (combined, score, lexical, i) in enumerate(reranked[:top_k], 1):
        f = funcs[i]
        print(f"// {rank}. cosine={score:.4f} jaccard={lexical:.4f}", f["declaration"])
        for idx, line in enumerate(f["content"].split("\n")):
            if idx > 15:
                break
            print(line)


def main():
    parser = ArgumentParser()
    sub = parser.add_subparsers(title="", required=True)
    index_parser = sub.add_parser("index")
    index_parser.add_argument("folder")
    index_parser.add_argument("-i", "--index", required=True)
    index_parser.set_defaults(func=lambda ns: index(Path(ns.folder), Path(ns.index)))
    search_parser = sub.add_parser("search")
    search_parser.add_argument("-q", "--query")
    search_parser.add_argument("-f", "--file")
    search_parser.add_argument("-i", "--index", required=True)
    search_parser.add_argument("-k", "--top-k", type=int, default=5)
    search_parser.set_defaults(
        func=lambda ns: search(ns.query or Path(ns.file), Path(ns.index), ns.top_k)
    )
    args = parser.parse_args()
    args.func(args)


if __name__ == "__main__":
    main()
