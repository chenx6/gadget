# X Card renderer

## Usage

Fetcher:

```bash
# Edit header.json to add request header
# Set TWITTER_LIST_ID environment variable
$ export TWITTER_LIST_ID=2101633191509622929
# Run!
$ uv run src/twitter.py
# Will put twitter json into data/tweet.db
```

Renderer:

```bash
# Put NotoColorEmoji.ttf, SourceHanSansCN-Medium.otf and SourceHanSansCN-VF.otf into font/ folder
# Build
$ RUSTFLAGS="-C target-cpu=native" cargo build --release
# Render
$ ./target/release/x_card_renderer data/tweet.json rendered.jpeg
```
