from json import dumps, loads
from pathlib import Path
from sqlite3 import connect
from subprocess import run
from tempfile import TemporaryDirectory
from time import sleep, time

from httpx import Client

RENDERER = "./target/release/x_card_renderer"
BASE_URL = "http://127.0.0.1:8080/v1/chat/completions"
MODEL = "Index-Translate-2B.Q4_K_S.gguf"
API_KEY = ""


def chat_completion(user_message: str) -> str:
    headers = {
        "Content-Type": "application/json",
    }
    if API_KEY:
        headers["Authorization"] = f"Bearer {API_KEY}"
    payload = {
        "model": MODEL,
        "messages": [
            {"role": "system", "content": ""},
            {"role": "user", "content": user_message},
        ],
        "temperature": 0,
        "chat_template_kwargs": {"enable_thinking": False},
    }
    with Client(timeout=60.0) as client:
        response = client.post(BASE_URL, headers=headers, json=payload)
        response.raise_for_status()
        data = response.json()
    return data["choices"][0]["message"]["content"]


def translate(content: str):
    return chat_completion(
        "请将以下日文文本翻译为中文，直接输出翻译结果，不要进行任何解释。\n\n" + content
    )


def main():
    rendered = []
    conn = connect("data/tweet.db")
    while True:
        cur = conn.execute(
            "SELECT tweet_id, content FROM tweet WHERE timestamp > ?",
            (int(time()) - 120,),
        )
        if rs := cur.fetchall():
            for tid, content in rs:
                if tid in rendered:
                    continue
                content_json = loads(content)
                full_text = content_json["full_text"]
                if not full_text.startswith("RT @"):
                    content_json["translated_text"] = translate(full_text)
                if retweet := content_json.get("retweet"):
                    retweet["translated_text"] = translate(retweet["full_text"])
                content = dumps(content_json)
                with TemporaryDirectory() as td:
                    tdp = Path(td)
                    tweet_file = tdp / "tweet.json"
                    with open(tweet_file, "w") as f:
                        f.write(content)
                    run([RENDERER, tweet_file, f"{tid}.jpeg"], check=False)
                    print(f"Rendering {tid} done")
                rendered.append(tid)
                if len(rendered) > 10:
                    rendered.pop(0)
        sleep(30)


if __name__ == "__main__":
    main()
