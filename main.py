from flask import Flask, render_template, request, session, redirect, url_for, flash
from threading import Thread
import os
import json
import smtplib
from email.mime.text import MIMEText

app = Flask(__name__)
app.secret_key = os.urandom(24)



def sendEmail(to):

    Email = "no.reply.smart.switch@gmail.com"    #"no-reply-kodaikanal-otp@gmail.com"
    Pwd =  "smart_switch_poop"           #"kodaikanal_otp123"


    sub = "SmartSwitch sensor status update."
    body = f"""
    Your switch is on however the sensor detected that theres no one in the room."""

    msg = MIMEText(body)


    msg["Subject" ] = sub   
    msg["From"] = Email
    msg["To"] = to[0]

    try:

        smtp_server = smtplib.SMTP_SSL('smtp.gmail.com', 465)
        smtp_server.ehlo()
        smtp_server.login(Email, Pwd)
        smtp_server.sendmail(Email, to, msg.as_string())
        smtp_server.close()
        print ("Email sent successfully!")
        return True

    except Exception as ex:

        print ("Something went wrong….",ex)
        return False




@app.route("/")
def Home():
    return render_template("home/index.html")

#nigger
@app.route("/login/", methods=["GET", "POST"])
def login():

    if request.method == "POST":

        users = json.load(open("users.json"))

        e = request.form["email"]
        p = request.form["password"]

        if e in users["users"]:

            if p == users["users"][e]["password"]:
                session.permanent = True
                session["user"] = e
                return redirect(url_for("control"))
            else:
                flash("Incorect email or password", "info")
                return redirect(url_for("login"))



        else:
            flash("Incorect email or password", "info")
            return redirect(url_for("login"))

    if "user" in session:
        return redirect(url_for("control"))


    return render_template("login/index.html")


@app.route("/createAccount/", methods=["GET", "POST"])
def createAccount():

    if request.method == "POST":
        
        users = json.load(open("users.json"))

        e = request.form["email"]
        p = request.form["password"]

        if e in users["users"]:
            flash("Account with this email exists", "info")
            return redirect(url_for("createAccount"))

        users["users"][e] = {"password" : p, "switches" : {}}

        open("users.json", "w").write(json.dumps(users, indent = 4))

        flash("Account Created", "info")
        return redirect(url_for("login"))


    return render_template("createAccount/index.html")


@app.route("/control", methods = ["POST", "GET"])
def control():
    if "user" not in session:
        flash("Log in first", "info")
        return redirect(url_for("login"))
        
    return render_template("control/index.html")


@app.route("/switch/<id>/<task>/<e>")
def switch(id, task, e):

    if task == "set":
        users = json.load(open("users.json"))

        state = users["users"][e]["switches"][id]

        if state == "0":
            state = "1"
        elif state == "1":
            state = "0"

        users["users"][session["user"]]["switches"][id] = state

        open("users.json", "w").write(json.dumps(users, indent = 4))

        return state

    if task == "get":

        users = json.load(open("users.json"))

        for u in users["users"]:
            print(u)
            if id in users["users"][u]["switches"]:
                state = users["users"][u]["switches"][id]

        print("req")

        return state

    if task == "add":
        usableSwitches = json.load(open("usableSwitches.json"))
        if id not in usableSwitches:
            return "ID does not exist"

        users = json.load(open("users.json"))

        users["users"][session["user"]]["switches"][id] = "1"
        usableSwitches.remove(id)

        open("users.json", "w").write(json.dumps(users, indent = 4))
        open("usableSwitches.json", "w").write(json.dumps(usableSwitches, indent = 4))

        return "ID Added"

    if task == "getAll":
        switches = json.load(open("users.json"))["users"][session["user"]]["switches"]
        
        return switches

    if task == "statusUpdate":
        users = json.load(open("users.json"))

        state = -1
        email = ""

        for u in users["users"]:
            if id in users["users"][u]["switches"]:
                email = u
                state = users["users"][u]["switches"][id]

        if state == "1":

            sendEmail(u)

            return "ok"

        return "ok"

    return "This page is not for you."
    
def run():
    app.run(host='0.0.0.0', debug=True)


def keep_alive():
    t = Thread(target=run)
    t.start()


if __name__ == "__main__":
    run()
