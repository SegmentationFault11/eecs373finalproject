"""This module is used to serve the js webapp"""
import requests, json
from flask import Flask, render_template, jsonify, Response
app = Flask(__name__)

API_DOMAIN = "http://localhost:8000/"

@app.route("/")
def web_page():
    return render_template("webapp.html")

@app.route("/<path>")
def forward_request(path):
    api_endpoint = API_DOMAIN + path
    print "Forwarding request to " + api_endpoint

    try:
        response_from_api_server = requests.get(api_endpoint).json()
        print "Got response from " + api_endpoint + ": ", response_from_api_server
        
        # if response is an array then convert it and return, else jsonify and
        # return
        response = {}
        if isinstance(response_from_api_server, list):
            response = Response(json.dumps(response_from_api_server), 
                    mimetype = "application/json")
        else:
            response = jsonify(response_from_api_server)
        return response

    except Exception as exc:
        print str(exc)
        jsonify ({"status" : str(exc)})

    jsonify ({"status" : "internal error"})
