from tomllib import load
from http.server import BaseHTTPRequestHandler, HTTPServer

from temperature import sys_temperature, smartctl_temperature


def load_config():
    with open("./config.toml", "rb") as f:
        config_file = load(f)
        return config_file


def get_homepage_content():
    page = """
    <!DOCTYPE html>
    <html>
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/picnic">
    </head>
    <body>
        <main style="max-width:800px; margin:0 auto">
    """
    for name, info in config["homepage"].items():
        url = info["url"]
        for varname, value in config["var"].items():
            url = url.replace("{" + varname + "}", value)
        page += f"""
        <div class="card">
            <header>{name}</header>
            <footer><a href="{url}">URL</a></footer>
        </div>
        """
    for ty, sensors in config["sensors"].items():
        page += '<div class="card">'
        if ty == "cpu":
            for sensor in sensors:
                page += f"<p>CPU {sensor} temperature: {sys_temperature(sensor)}°C</p>"
        elif ty == "hard_disk":
            for disk in sensors:
                page += (
                    f"<p>Disk {disk} temperature: {smartctl_temperature(disk)}°C</p>"
                )
        page += "</div>"
    page += "</main></body></html>"
    return page


def get_server(config: dict):
    class RequestHandler(BaseHTTPRequestHandler):
        def do_GET(self):
            self.send_response(200)
            self.end_headers()
            self.wfile.write(get_homepage_content().encode())

    network = config["network"]
    host, port = network["host"], network["port"]
    print(f"Listen on {host}:{port}")
    server = HTTPServer((host, port), RequestHandler)
    return server


config = load_config()
server = get_server(config)
server.serve_forever()