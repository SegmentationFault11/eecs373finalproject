import json
from flask import Response, request, jsonify
from functools import wraps

def api_jsonify(fn):
    """Use this function as a decorator on a function that returns a
    Python dictionary or something that is json.dumps()-able.  This then takes
    the response from a call to json.dumps() and returns it with the
    appropriate mimetype set"""
    @wraps(fn)
    def function_wrapper(*args, **kwargs):

        # Check if the data is in array format
        if request.method == "POST":

            if request.json is None:
                print request.json
                response = jsonify({"status": "Please provide some data as a "
                    "json array"})
                response.status_code = 400
                return response
            if not isinstance(request.json, list):
                print request.json
                response = jsonify({"status": "Please provide the data as an "
                    "array"})
                response.status_code = 400
                return response

        # return the response object constructed with the json.dumps from the
        # call to another function that returns an array or a simple
        # dictionary
        response_from_fn = fn(*args, **kwargs)
        if isinstance(response_from_fn, Response):
            print "Returned : ", response_from_fn.__dict__
            return response_from_fn
        else:
            response_to_return = Response(json.dumps(response_from_fn), mimetype = \
                    "application/json")
            print "Returned : ", response_to_return.__dict__
            return response_to_return

    return function_wrapper

