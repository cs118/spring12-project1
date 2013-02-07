#!/usr/local/cs/bin/python

from threading import Thread
from httplib import HTTPConnection
from BaseHTTPServer import BaseHTTPRequestHandler,HTTPServer
from datetime import datetime, timedelta
from bcolor import bcolors
import sys
import time

class TestHandler(BaseHTTPRequestHandler):

    def do_GET(self):
        if self.path == "/basic":
            cdata = open("./basic", "r").read()
        if self.path == "/basic2":
            cdata = open("./basic2", "r").read()
        if self.path == "/basic3":
            cdata = open("./basic3", "r").read()
            time.sleep(3)
        if self.path == "/cacheTest":
            cdata = str(time.time())

        size = len(cdata)
        expireDate=(datetime.now()+timedelta(days=1)).strftime("%a, %d %b %Y %H:%M:%S GMT")
        lastModify=(datetime.now()+timedelta(days=-1)).strftime("%a, %d %b %Y %H:%M:%S GMT")
        self.send_response(200)
        self.send_header('Content-Type','text/html')
        self.send_header('Content-Length', str(size))
        self.send_header('Expires',expireDate)
        self.send_header('Last-Modified', lastModify)
        if self.close_connection == True:
            self.send_header('Connection', 'close')
        self.end_headers()
        self.wfile.write(cdata)

        return

class ServerThread (Thread):
    def __init__(self, port):
        Thread.__init__(self)
        self.port = port

    def run(self):
        try:
            TestHandler.protocol_version = "HTTP/1.1"
            self.server = HTTPServer(('', self.port), TestHandler)
            self.server.serve_forever()
        except KeyboardInterrupt:
            self.server.socket.close()                                                                                                                                                       
    
class ClientThread (Thread):
    def __init__(self, proxy, url, file):
        Thread.__init__(self)
        self.proxy = proxy
        self.url = url
        self.file = file
        self.result = False
        self.data = ""

    def run(self):

        if self.file:
            dataFile = open(self.file, "r")
            cdata = dataFile.read()
        
            conn = HTTPConnection(self.proxy)
            conn.request("GET", self.url)
            resp = conn.getresponse()
            rdata = resp.read()

            if rdata == cdata:
                self.result = True
            self.data = rdata
            conn.close()
            dataFile.close()
        else:
            conn = HTTPConnection(self.proxy)
            conn.request("GET", self.url)
            resp = conn.getresponse()
            rdata = resp.read()
            
            if resp.status == httplib.OK:
                self.result = True
            conn.close()

class ClientPersistThread(Thread):
    def __init__(self, proxy, url, file, url2, file2):
        Thread.__init__(self)
        self.proxy = proxy
        self.url = url
        self.file = file
        self.url2 = url2
        self.file2 = file2
        self.result = False

    def run(self):
        conn = HTTPConnection(self.proxy)
        tmpFlag = True

        dataFile = open(self.file, "r")
        cdata = dataFile.read()
        dataFile = open(self.file2, "r")
        cdata2 = dataFile.read()

        conn.request("GET", self.url)
        resp = conn.getresponse()
        rdata = resp.read()
        if rdata != cdata:
            tmpFlag = False
            
        if resp.will_close == True:
            tmpFlag = False

        connHdrs = {"Connection": "close"}
        conn.request("GET", self.url2, headers=connHdrs)

        resp = conn.getresponse()
        rdata2 = resp.read()
        if rdata2 != cdata2:
            tmpFlag = False

        if resp.will_close == False:
            tmpFlag = False

        if tmpFlag == True:
            self.result = True
        conn.close()
        dataFile.close()


conf = open("./portconf", "r")
pport  = conf.readline().rstrip().split(':')[1]
sport1 = conf.readline().rstrip().split(':')[1]
sport2 = conf.readline().rstrip().split(':')[1]

b1 = open("./basic", "w")
b1.write("basic\n")
b2 = open("./basic2", "w")
b2.write("aloha\n")
b3 = open("./basic3", "w")
b3.write("cat\n")

b1.close()
b2.close()
b3.close()

server1 = ServerThread(int(sport1))
server2 = ServerThread(int(sport2))

server1.start()
server2.start()


client1 = ClientThread("127.0.0.1:" + pport, "http://127.0.0.1:" + sport1 + "/basic", "./basic")
client1.start()
client1.join()
if client1.result:
    print "Basic object fetching: [" + bcolors.PASS + "PASSED" + bcolors.ENDC + "]" 
else: 
    print "Basic object fetching: [" + bcolors.FAIL + "FAILED" + bcolors.ENDC + "]" 

client2 = ClientPersistThread("127.0.0.1:" + pport, "http://127.0.0.1:" + sport1 + "/basic", "./basic", "http://127.0.0.1:" + sport1 + "/basic2", "./basic2")
client2.start()
client2.join()
if client2.result:
    print "Persistent Connection: [" + bcolors.PASS + "PASSED" + bcolors.ENDC + "]"
else:
    print "Persistent Connection: [" + bcolors.FAIL + "FAILED" + bcolors.ENDC + "]"

client3 = ClientThread("127.0.0.1:" + pport, "http://127.0.0.1:"+ sport1 +"/basic3", "./basic3")
client4 = ClientThread("127.0.0.1:" + pport, "http://127.0.0.1:"+ sport2 +"/basic3", "./basic3")

start = time.time()
client3.start()
client4.start()
 
client3.join()
client4.join()
end = time.time()

r = False
datafile = open("./basic3", "r")
cdata = datafile.read()
if(end - start) < 4 and client3.data == cdata and client4.data == cdata:
    r = True
if r:
    print "Concurrent Connection: [" + bcolors.PASS + "PASSED" + bcolors.ENDC + "]"
else:
    print "Concurrent Connection: [" + bcolors.FAIL + "FAILED" + bcolors.ENDC + "]"

client5 = ClientThread("127.0.0.1:" + pport, "http://127.0.0.1:"+sport1+"/cacheTest", "./basic")
client5.start()
client5.join()
time.sleep(2)
client6 = ClientThread("127.0.0.1:" + pport, "http://127.0.0.1:"+sport1+"/cacheTest", "./basic")
client6.start()
client6.join()
r = False
if client5.data == client6.data and client5.data != "":
    r = True
if r:
    print "Caching: [" + bcolors.PASS + "PASSED" + bcolors.ENDC + "]"
else:
    print "Caching: [" + bcolors.FAIL + "FAILED" + bcolors.ENDC + "]"

server1.server.shutdown()
server2.server.shutdown()
