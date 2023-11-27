import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

from datetime import datetime

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    username = db.execute(
        "SELECT username,cash FROM users WHERE id = ?", session["user_id"]
    )
    user = db.execute(
        "SELECT symbol,SUM(quantity) FROM purchases WHERE username = ? GROUP BY symbol",
        username[0]["username"],
    )
    current_cash = username[0]["cash"]
    grand_value = username[0]["cash"]
    for stock in user:
        stock["current_price"] = lookup(stock["symbol"])["price"]
        stock["total_value"] = stock["SUM(quantity)"] * stock["current_price"]
        grand_value += stock["total_value"]
    return render_template(
        "index.html", user=user, current_cash=current_cash, grand_value=grand_value
    )


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "GET":
        return render_template("buy.html")
    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")
        if not symbol:
            return apology("Enter a symbol")
        if not shares:
            return apology("Enter shares")
        if not shares.isdigit():
            return apology("Enter a valid number of shares")
        shares = int(shares)
        if shares <= 0:
            return apology("Enter a valid number of shares")

        symbol = symbol.upper()
        price = lookup(symbol)
        if not price:
            return apology("Invalid symbol")
        user_data = db.execute(
            "SELECT username, cash FROM users WHERE id = ?", int(session["user_id"])
        )
        if price["price"] * shares > user_data[0]["cash"]:
            return apology("Not enough cash")
        date_time = datetime.now()
        db.execute(
            "INSERT INTO records (username,symbol,price,quantity,date,transaction_type) VALUES (?,?,?,?,?,?)",
            user_data[0]["username"],
            symbol,
            price["price"],
            shares,
            date_time,
            "BUY",
        )
        balance = user_data[0]["cash"] - price["price"] * shares
        db.execute(
            "UPDATE users SET cash = ? WHERE username = ?",
            balance,
            user_data[0]["username"],
        )
        purchase_data = db.execute(
            "SELECT quantity FROM purchases WHERE username = ? AND symbol = ?",
            user_data[0]["username"],
            symbol,
        )
        if not purchase_data:
            db.execute(
                "INSERT INTO purchases (username,symbol,price,quantity,date) VALUES (?,?,?,?,?)",
                user_data[0]["username"],
                symbol,
                price["price"],
                shares,
                date_time,
            )
        else:
            updated_shares = purchase_data[0]["quantity"] + shares
            db.execute(
                "UPDATE purchases SET quantity = ? WHERE username = ? AND symbol = ?",
                updated_shares,
                user_data[0]["username"],
                symbol,
            )
        return redirect("/")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    username = db.execute("SELECT username FROM users WHERE id = ?", session["user_id"])
    user = db.execute(
        "SELECT * FROM records WHERE username = ?", username[0]["username"]
    )
    return render_template("history.html", user=user)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "GET":
        return render_template("quote.html")
    if request.method == "POST":
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("enter a symbol")
        price = lookup(symbol)
        if not price:
            return apology("Invalid symbol")
        return render_template("quoted.html", price=price)


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "GET":
        return render_template("register.html")
    elif request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")
        if not username:
            return apology("Enter a username")
        else:
            existing_user = db.execute(
                "SELECT username FROM users WHERE username = ?", username
            )

        if not password or not confirmation:
            return apology("Enter the password")

        if existing_user:
            return apology("Username already exists")

        if password != confirmation:
            return apology("The passwords do not match")

        db.execute(
            "INSERT INTO users (username,hash) VALUES (?,?)",
            username,
            generate_password_hash(password),
        )
        return redirect("/")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    username = db.execute(
        "SELECT username,cash FROM users WHERE id = ?", session["user_id"]
    )
    stocks = db.execute(
        "SELECT symbol,SUM(quantity) FROM purchases WHERE username = ? GROUP BY symbol",
        username[0]["username"],
    )
    if request.method == "GET":
        return render_template("sell.html", stocks=stocks)
    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")
        if not symbol:
            return apology("Enter a symbol")
        if not shares:
            return apology("Enter shares")
        if not shares.isdigit():
            return apology("Enter a valid number of shares")
        shares = int(shares)
        if shares <= 0:
            return apology("Enter a valid number of shares")

        stock_found = False
        shares_owned = 0
        for stock in stocks:
            if symbol in stock["symbol"]:
                stock_found = True
                shares_owned = stock["SUM(quantity)"]
        if not stock_found:
            return apology("You don't own the stock")

        if shares > shares_owned:
            return apology("You do not own sufficient shares")

        price = lookup(symbol)
        balance = username[0]["cash"] + price["price"] * shares

        date_time = datetime.now()
        db.execute(
            "INSERT INTO records (username,symbol,price,quantity,date,transaction_type) VALUES (?,?,?,?,?,?)",
            username[0]["username"],
            symbol,
            price["price"],
            shares,
            date_time,
            "SELL",
        )
        db.execute(
            "UPDATE users SET cash = ? WHERE username = ?",
            balance,
            username[0]["username"],
        )
        updated_shares = stocks[0]["SUM(quantity)"] - shares
        if not updated_shares:
            db.execute(
                "DELETE FROM purchases WHERE username = ? AND symbol = ?",
                username[0]["username"],
                symbol,
            )
        else:
            db.execute(
                "UPDATE purchases SET quantity = ? WHERE username = ? AND symbol = ?",
                updated_shares,
                username[0]["username"],
                symbol,
            )
        return redirect("/")


@app.route("/addcash", methods=["GET", "POST"])
@login_required
def addcash():
    """Add additional cash to account"""
    if request.method == "GET":
        return render_template("addcash.html")
    if request.method == "POST":
        cash = request.form.get("cash")
        if not cash:
            return apology("Enter an amount")
        if not cash.isdigit():
            return apology("Enter a valid amount")
        cash = int(cash)
        if cash < 0:
            return apology("Enter a valid amount")

        username = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
        new_cash = cash + username[0]["cash"]
        db.execute(
            "UPDATE users SET cash = ? WHERE id = ?", new_cash, session["user_id"]
        )
        return redirect("/")

