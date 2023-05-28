# Python 3 server example

from tree import RGBXmasTree
from colorzero import Color, Hue
from time import sleep
from http.server import BaseHTTPRequestHandler, HTTPServer
import random

tree = RGBXmasTree()

colors = [Color('red'), Color('green'), Color('blue')] # add more if you like

#try:
#    while True:
#        for color in colors:
#            tree.color = color
#            sleep(1)
#except KeyboardInterrupt:
#    tree.close()

hostName = "192.168.86.197"
serverPort = 8080
def random_color():
    r = random.random()
    g = random.random()
    b = random.random()
    return (r, g, b)

class MyServer(BaseHTTPRequestHandler):
    def do_GET(self):
        self.send_response(200)
        self.send_header("Content-type", "text/html")
        self.end_headers()
        self.wfile.write(bytes("<html><head><title>https://pythonbasics.org</title></head>", "utf-8"))
        self.wfile.write(bytes("<p>Request: %s</p>" % self.path, "utf-8"))
        self.wfile.write(bytes("<body>", "utf-8"))
        self.wfile.write(bytes("<p>This is an example web server.</p>", "utf-8"))
        self.wfile.write(bytes("</body></html>", "utf-8"))
        if self.path == "/rgb":
           for color in colors:
               tree.color = color
               sleep(1)
        if self.path == "/off":
           tree.color = Color(0,0,0)
        if self.path == "/red":
           tree.color = Color('red')
        if self.path == "/green":
           tree.color = Color('green')
        if self.path == "/blue":
           tree.color = Color('blue')
        if self.path == "/next":
           tree.color += Hue(deg=10)
        if self.path == "/rand":
           for x in range(10):
              pixel = random.choice(tree)
              pixel.color = random_color()

if __name__ == "__main__":        
    webServer = HTTPServer((hostName, serverPort), MyServer)
    print("Server started http://%s:%s" % (hostName, serverPort))
    try:
        webServer.serve_forever()
    except KeyboardInterrupt:
        pass

    webServer.server_close()
    print("Server stopped.")
