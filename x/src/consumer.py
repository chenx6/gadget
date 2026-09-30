from pathlib import Path
from sqlite3 import connect
from subprocess import run
from tempfile import TemporaryDirectory
from time import sleep, time

RENDERER = "./target/release/x_card_renderer"


def main():
    conn = connect("data/tweet.db")
    while True:
        cur = conn.execute(
            "SELECT content FROM tweet WHERE timestamp > ?", (int(time()) - 120,)
        )
        if rs := cur.fetchall():
            for (t,) in rs:
                with TemporaryDirectory() as td:
                    tdp = Path(td)
                    tweet_file = tdp / "tweet.json"
                    with open(tweet_file, "w") as f:
                        f.write(t)
                    run([RENDERER, tweet_file, "rendered.jpeg"], check=False)
        sleep(60)


if __name__ == "__main__":
    main()
