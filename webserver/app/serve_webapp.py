from flask import Flask, render_template
app = Flask(__name__)

@app.route("/")
def web_page():
    return render_template("webapp.html")

if __name__ == "__main__":
    app.run(debug = True)
