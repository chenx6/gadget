from argparse import ArgumentParser
from collections import Counter, defaultdict
from json import dumps
from pathlib import Path
from sqlite3 import connect
from sys import exit
from typing import TYPE_CHECKING, NamedTuple

from rzpipe import open as rzopen
from xxhash import xxh3_64_intdigest

if TYPE_CHECKING:
    from sqlite3 import Connection


class Function(NamedTuple):
    name: str
    offset: int
    size: int
    callrefs: list[dict[str, int | str]]


StringAnalyseResult = tuple[int, str, int]

DB_PATH = Path("fingerprint.db")


def xxh3_signed(data: bytes) -> int:
    """
    xxh3_64_intdigest returns an unsigned 64-bit int, but SQLite INTEGER
    only holds signed 64-bit values. Wrap it into the signed range.
    """
    h = xxh3_64_intdigest(data)
    return h - (1 << 64) if h >= (1 << 63) else h


def init_db(conn: "Connection"):
    """
    hash_string(hash, ...) <=1=> hash_info(hash, info_id) <=n=> info(id, ...)
    """
    conn.executescript(
        """
        CREATE TABLE IF NOT EXISTS info (
            id INTEGER PRIMARY KEY,
            library TEXT,
            version TEXT,
            function TEXT,
            offset INTEGER,
            callrefs TEXT,
            UNIQUE(library, version, function)
        );
        CREATE TABLE IF NOT EXISTS hash_info (
            hash INTEGER,
            info_id INTEGER,
            UNIQUE(info_id, hash)
        );
        CREATE TABLE IF NOT EXISTS hash_string (
            hash INTEGER,
            string TEXT
        );
        CREATE INDEX IF NOT EXISTS idx_hash_info_hash ON hash_info(hash);
        CREATE INDEX IF NOT EXISTS idx_hash_string_hash ON hash_string(hash);
        """
    )
    conn.commit()


def store_analysis(
    conn: "Connection",
    library: str,
    version: str,
    funcs: list[Function],
    strings: list[StringAnalyseResult],
):
    # Add functions
    func_id: dict[str, int] = {}
    for func in funcs:
        cur = conn.execute(
            """
            INSERT OR IGNORE INTO info (library, version, function, offset, callrefs) VALUES (?, ?, ?, ?, ?)
            RETURNING id
            """,
            (library, version, func.name, func.offset, dumps(func.callrefs)),
        )
        iid = cur.fetchone()
        if not iid:
            cur = conn.execute(
                "SELECT id FROM info WHERE library = ? AND version = ? AND function = ?",
                (library, version, func.name),
            )
            iid = cur.fetchone()
        if iid:
            func_id[func.name] = iid
    for idx, string, string_hash in strings:
        # Connect string and function
        func = funcs[idx]
        iid = func_id[func.name]
        # Add string and hash into hash_info and hash_string table
        conn.execute(
            "INSERT OR IGNORE INTO hash_info (info_id, hash) VALUES (?, ?)",
            (iid, string_hash),
        )
        cur = conn.execute("SELECT 1 FROM hash_string WHERE hash = ?", (string_hash,))
        if cur.fetchall():
            continue
        conn.execute(
            "INSERT INTO hash_string (hash, string) VALUES (?, ?)",
            (string_hash, string),
        )
    conn.commit()


def recover_funcname(
    conn: "Connection",
    info: tuple[str, str],
    funcs: list[Function],
    strings: list[StringAnalyseResult],
):
    matched = set()
    for func_idx, string, h in strings:
        # Recover function name by using string match
        if h in matched:
            continue
        matched.add(h)
        cur = conn.execute(
            """
            SELECT function
            FROM info, hash_info
            WHERE library = ? AND version = ? AND hash = ? AND info.id = hash_info.info_id
            """,
            (
                info[0],
                info[1],
                h,
            ),
        )
        res = cur.fetchall()
        if not res or len(res) > 1:
            # If current hash mapped to multiple function, ignore it
            continue
        orig_func_name = res[0][0]
        print(funcs[func_idx].name, "=>", orig_func_name, string.encode())


def query_library(
    conn: "Connection", funcs: list[Function], strings: list[StringAnalyseResult]
):
    cnt: Counter[tuple[str, str]] = Counter()
    for h in {i for _, _, i in strings}:
        # Calculate (library,version) match count
        cur = conn.execute(
            """
            SELECT library, version
            FROM info, hash_info
            WHERE hash = ? AND info.id = hash_info.info_id
            """,
            (h,),
        )
        res = cur.fetchall()
        for r in res:
            cnt[r] += 1
    for info, count in cnt.items():
        # Get all hash count for current library and version
        cur = conn.execute(
            """
            SELECT COUNT(hash)
            FROM info, hash_info
            WHERE library = ? AND version = ? AND info.id = hash_info.info_id
            """,
            info,
        )
        res = cur.fetchone()
        total = res[0]
        # Check confidence
        confidence = count / total
        if confidence < 0.4:
            continue
        print(info, confidence, count, total)
        recover_funcname(conn, info, funcs, strings)


def analysis(file: str) -> tuple[list[Function], list[StringAnalyseResult]] | None:
    with rzopen(file) as p:
        p.cmd("aaa")
        # Functions
        res = p.cmdj("aflj")
        if not res:
            return
        funcs = [
            Function(i["name"], i["offset"], i["size"], i.get("callrefs", []))
            for i in res
        ]
        data_to_funcs: dict[int, list[int]] = defaultdict(list)
        for func_idx, i in enumerate(res):
            # Process dataref to get data => functions index
            for ref in i.get("datarefs") or []:
                data_to_funcs[ref["to"]].append(func_idx)
        # Strings
        res = p.cmdj("izj")
        if not res:
            return
        strings: list[StringAnalyseResult] = []
        for v in res:
            for func_idx in data_to_funcs.get(v["vaddr"], ()):
                s = v["string"]
                strings.append((func_idx, s, xxh3_signed(s.encode())))
        return (funcs, strings)


def extract_fingerprint(file: str, lib: str, version: str):
    res = analysis(file)
    if not res:
        return
    funcs, strings = res
    conn = connect(str(DB_PATH))
    init_db(conn)
    store_analysis(conn, lib, version, funcs, strings)


def detect_fingerprint(file: str):
    res = analysis(file)
    if not res:
        return
    funcs, strings = res
    conn = connect(str(DB_PATH))
    query_library(conn, funcs, strings)


if __name__ == "__main__":
    parser = ArgumentParser()
    parser.add_argument("option", choices=["extract", "detect"])
    parser.add_argument("file")
    parser.add_argument("--info")
    args = parser.parse_args()
    match args.option:
        case "extract":
            if not args.info:
                parser.error("--info LIB/VERSION is required for extract")
            lib, version = args.info.split("/")
            extract_fingerprint(args.file, lib, version)
        case "detect":
            if not DB_PATH.exists():
                print(DB_PATH, "not exist")
                exit()
            detect_fingerprint(args.file)
