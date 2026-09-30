# /// script
# dependencies = [
#   "httpx[http2]",
# ]
# ///
from datetime import datetime
from functools import partial
from json import dumps, load
from logging import INFO, Formatter, StreamHandler, getLogger
from os import environ
from pathlib import Path
from sqlite3 import connect
from time import sleep
from typing import TYPE_CHECKING

from httpx import Client, get

if TYPE_CHECKING:
    from sqlite3 import Connection

URL = "https://x.com/i/api/graphql/qIrerw_1oakhehcLoFbaOw/ListLatestTweetsTimeline"
LIST_ID = environ["TWITTER_LIST_ID"]


def get_logger():
    logger = getLogger("twitter")
    logger.setLevel(INFO)
    handler = StreamHandler()
    handler.setFormatter(Formatter("%(asctime)s %(levelname)s %(name)s: %(message)s"))
    logger.addHandler(handler)
    return logger


def list_timeline():
    params = {
        "variables": f'{{"listId":"{LIST_ID}","count":20}}',
        "features": '{"rweb_video_screen_enabled":false,"rweb_cashtags_enabled":true,"profile_label_improvements_pcf_label_in_post_enabled":true,"responsive_web_profile_redirect_enabled":true,"rweb_tipjar_consumption_enabled":false,"verified_phone_label_enabled":false,"creator_subscriptions_tweet_preview_api_enabled":true,"responsive_web_graphql_timeline_navigation_enabled":true,"premium_content_api_read_enabled":false,"communities_web_enable_tweet_community_results_fetch":true,"c9s_tweet_anatomy_moderator_badge_enabled":true,"responsive_web_grok_analyze_button_fetch_trends_enabled":false,"responsive_web_grok_analyze_post_followups_enabled":true,"rweb_cashtags_composer_attachment_enabled":true,"responsive_web_jetfuel_frame":true,"rweb_sports_post_context_enabled":false,"responsive_web_grok_share_attachment_enabled":true,"responsive_web_grok_annotations_enabled":true,"articles_preview_enabled":true,"responsive_web_edit_tweet_api_enabled":true,"rweb_conversational_replies_downvote_enabled":false,"graphql_is_translatable_rweb_tweet_is_translatable_enabled":true,"view_counts_everywhere_api_enabled":true,"longform_notetweets_consumption_enabled":true,"responsive_web_twitter_article_tweet_consumption_enabled":true,"content_disclosure_indicator_enabled":true,"content_disclosure_ai_generated_indicator_enabled":true,"responsive_web_grok_show_grok_translated_post":true,"responsive_web_grok_analysis_button_from_backend":true,"post_ctas_fetch_enabled":false,"freedom_of_speech_not_reach_fetch_enabled":true,"standardized_nudges_misinfo":true,"tweet_with_visibility_results_prefer_gql_limited_actions_policy_enabled":true,"longform_notetweets_rich_text_read_enabled":true,"longform_notetweets_inline_media_enabled":false,"responsive_web_nested_quote_preview_enabled":false,"responsive_web_grok_image_annotation_enabled":true,"responsive_web_grok_imagine_annotation_enabled":true,"responsive_web_grok_community_note_auto_translation_is_enabled":true,"responsive_web_enhance_cards_enabled":false}',
    }
    with open("headers.json") as f:
        headers = load(f)
    with Client(http2=True, follow_redirects=True) as client:
        response = client.get(URL, params=params, headers=headers)
        response.raise_for_status()
        return response.json()


def extract_original_tweet(rs_result: dict):
    user_result = rs_result["result"]["core"]["user_results"]["result"]
    avatar_url = user_result["avatar"]["image_url"]
    user_name = user_result["core"]["name"]
    screen_name = user_result["core"]["screen_name"]
    legacy = rs_result["result"]["legacy"]
    full_text = legacy["full_text"]
    tweet_id = legacy["id_str"]
    created_at = legacy["created_at"]
    media_urls = []
    for media in legacy.get("extended_entities", {}).get("media", []):
        if media.get("type") == "photo":
            media_urls.append(media.get("media_url_https", ""))
            break
    return {
        "tweet_id": tweet_id,
        "created_at": created_at,
        "lang": legacy.get("lang"),
        "user_name": user_name,
        "screen_name": screen_name,
        "avatar_url": avatar_url,
        "full_text": full_text,
        "hashtags": [],
        "urls": [],
        "media_urls": media_urls,
        "favorite_count": 0,
        "retweet_count": 0,
        "reply_count": 0,
        "quote_count": 0,
        "views": "0",
    }


def extract_tweets(data: dict):
    """从 Twitter GraphQL 响应中提取推文内容"""
    tweets = []
    try:
        # instructions = data["data"]["home"]["home_timeline_urt"]["instructions"]
        instructions = data["data"]["list"]["tweets_timeline"]["timeline"][
            "instructions"
        ]
    except (KeyError, TypeError):
        return tweets
    for instruction in instructions:
        # 只处理包含 entries 的指令
        entries = instruction.get("entries", [])
        for entry in entries:
            content = entry.get("content", {})
            # 只处理推文类型的 entry
            if content.get("entryType") != "TimelineTimelineItem":
                continue
            item_content = content.get("itemContent", {})
            if item_content.get("itemType") != "TimelineTweet":
                continue
            result = item_content.get("tweet_results", {}).get("result", {})
            if not result:
                continue
            legacy = result.get("legacy", {})
            user_result = (
                result.get("core", {}).get("user_results", {}).get("result", {})
            )
            # 提取用户信息
            user_core = user_result.get("core", {})
            user_name = user_core.get("name", "")
            screen_name = user_core.get("screen_name", "")
            avatar_url = user_result.get("avatar", {}).get("image_url", "")
            # 提取推文文本
            full_text = legacy.get("full_text", "")
            # 提取实体（hashtags、urls、media）
            entities = legacy.get("entities", {})
            hashtags = [h.get("text", "") for h in entities.get("hashtags", [])]
            urls = [u.get("expanded_url", "") for u in entities.get("urls", [])]
            # 提取媒体图片
            media_urls = []
            for media in legacy.get("extended_entities", {}).get("media", []):
                if media.get("type") == "photo":
                    media_urls.append(media.get("media_url_https", ""))
            retweet = None
            if (rs_result := legacy.get("retweeted_status_result")) or (
                rs_result := result.get("quoted_status_result")
            ):
                retweet = extract_original_tweet(rs_result)
            # 提取统计数据
            tweet_info = {
                "tweet_id": legacy.get("id_str", result.get("rest_id", "")),
                "created_at": legacy.get("created_at", ""),
                "lang": legacy.get("lang", ""),
                "user_name": user_name,
                "screen_name": screen_name,
                "avatar_url": avatar_url,
                "full_text": full_text,
                "hashtags": hashtags,
                "urls": urls,
                "media_urls": media_urls,
                "favorite_count": legacy.get("favorite_count", 0),
                "retweet_count": legacy.get("retweet_count", 0),
                "reply_count": legacy.get("reply_count", 0),
                "quote_count": legacy.get("quote_count", 0),
                "views": result.get("views", {}).get("count", ""),
                "retweet": retweet,
            }
            tweets.append(tweet_info)
    return tweets


def get_output(url: str):
    name = url.split("/")[-1]
    path = Path("data") / name
    if path.exists():
        return str(path)
    resp = get(url)
    with open(path, "wb") as f:
        f.write(resp.content)
    return str(path)


def save_pics(tweet: dict):
    tweet["avatar_url"] = get_output(tweet["avatar_url"])
    tweet["media_urls"] = [get_output(i) for i in tweet["media_urls"]]


def init_db(conn: "Connection"):
    conn.execute(
        """
        CREATE TABLE IF NOT EXISTS tweet (
            id INTEGER PRIMARY KEY,
            tweet_id TEXT,
            screen_name TEXT,
            timestamp INTEGER,
            content TEXT
        )
        """
    )
    conn.commit()


def tweet_existed(conn: "Connection", tweet_id: str):
    cur = conn.execute("SELECT 1 FROM tweet WHERE tweet_id = ?", (tweet_id,))
    return cur.fetchall() != []


def add_tweet(conn: "Connection", tweet: dict):
    dt = datetime.strptime(tweet["created_at"], "%a %b %d %H:%M:%S %z %Y")
    timestamp = int(dt.timestamp())
    conn.execute(
        "INSERT INTO tweet (tweet_id, screen_name, timestamp, content) VALUES (?, ?, ?, ?)",
        (tweet["tweet_id"], tweet["screen_name"], timestamp, dumps(tweet)),
    )
    conn.commit()


def update_task(conn: "Connection"):
    data = list_timeline()
    tweets = extract_tweets(data)
    for t in tweets:
        if not tweet_existed(conn, t["tweet_id"]):
            try:
                save_pics(t)
                if retweet := t.get("retweet"):
                    save_pics(retweet)
            except Exception as e:  # noqa: BLE001
                logger.error("Save picture failed: %s", e)
                continue
            add_tweet(conn, t)
            logger.info("Add tweet %s", t)


def loop(interval: float, func):
    while True:
        try:
            logger.info("Updating")
            func()
            logger.info("Update done")
        except Exception:
            logger.exception("Exception occured")
        sleep(interval)


def main():
    conn = connect("data/tweet.db")
    conn.execute("PRAGMA journal_mode=WAL;")
    init_db(conn)
    loop(120.0, partial(update_task, conn))


logger = get_logger()
if __name__ == "__main__":
    main()
