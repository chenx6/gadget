from pathlib import Path
from sqlite3 import connect
from subprocess import run
from tempfile import TemporaryDirectory
from time import sleep, time

RENDERER = "./target/release/x_card_renderer"


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
