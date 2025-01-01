from flask import Flask, jsonify, request, make_response, session  # type: ignore
import secrets
from flask_mysqldb import MySQL  # type: ignore
import os
from flask_cors import CORS  # type: ignore
import logging

# Set up logging at the top of your app.py
logging.basicConfig(
    level=logging.INFO, format="%(asctime)s [%(levelname)s] %(message)s"
)

app = Flask(__name__)
app.secret_key = os.environ.get(
    "SECRET_KEY", "dev-key-for-local-testing"
)  # Set a secret key for sessions

# Updated CORS configuration
CORS(
    app,
    resources={
        r"/*": {
            "origins": [
                "http://192.168.1.240",
                "http://192.168.4.1",
                "http://localhost",
            ],
            "methods": ["GET", "POST", "OPTIONS"],
            "allow_headers": ["Content-Type", "Authorization"],
            "expose_headers": ["Content-Type", "Authorization", "Set-Cookie"],
            "supports_credentials": True,
        }
    },
)


# MySQL configurations - using your provided credentials
# (but please change these ASAP for security!)
app.config["MYSQL_HOST"] = os.environ.get("MYSQL_HOST")
app.config["MYSQL_USER"] = os.environ.get("MYSQL_USER")
app.config["MYSQL_PASSWORD"] = os.environ.get("MYSQL_PASSWORD")
app.config["MYSQL_DB"] = os.environ.get("MYSQL_DB")

mysql = MySQL(app)


@app.route("/")
def index():
    try:
        cur = mysql.connection.cursor()
        # Test the connection
        cur.execute("SELECT 1")
        cur.close()
        return "Database connection successful! Welcome to the Automatic Watering System API"
    except Exception as e:
        return jsonify({"error": f"Database connection failed: {str(e)}"}), 500


@app.route("/plantNode/<mac_address>", methods=["GET"])
def get_plant(mac_address):
    try:
        cur = mysql.connection.cursor()
        # Remove the database name from the query since it's specified in MYSQL_DB config
        cur.execute(
            "SELECT macAddress, nodeName FROM automatic_watering_system.plant_nodes WHERE macAddress = %s",
            (mac_address,),
        )
        plant = cur.fetchone()
        cur.close()

        if plant:
            return jsonify({"mac_address": plant[0], "name": plant[1]})
        else:
            return jsonify({"error": "Plant not found"}), 404

    except Exception as e:
        return jsonify({"error": f"Database error: {str(e)}"}), 500


@app.route("/checkConnection", methods=["GET"])
def check_connection():
    try:
        cur = mysql.connection.cursor()
        # Test the connection
        cur.execute("SELECT 1")
        cur.close()
        return jsonify({"success": True})
    except Exception as e:
        return jsonify({"error": f"Database connection failed: {str(e)}"}), 500


@app.route("/plantNode/select", methods=["GET"])
def select_plant():
    try:
        # Get search parameters
        mac_address = request.args.get("macAddress")
        node_name = request.args.get("nodeName")

        # Get threshold types (handles multiple values)
        thresholds = request.args.getlist("thresholds[]")

        # Get and validate requested return fields
        fields = [
            f.strip() for f in request.args.get("fields", "").split(",") if f.strip()
        ]

        # Build SQL query dynamically
        query = "SELECT {} FROM automatic_watering_system.plant_nodes WHERE 1=1"
        params = []

        # Add requested fields or select all
        select_fields = "*" if not fields else ", ".join(fields)
        query = query.format(select_fields)

        # Add search conditions
        if mac_address:
            query += " AND macAddress = %s"
            params.append(mac_address)
        if node_name:
            query += " AND nodeName = %s"
            params.append(node_name)
        if thresholds:
            query += " AND threshold_type IN ({})".format(
                ",".join(["%s"] * len(thresholds))
            )
            params.extend(thresholds)

        cur = mysql.connection.cursor()
        cur.execute(query, tuple(params))
        results = cur.fetchall()

        # Format response based on requested fields
        if fields:
            response = [
                {fields[i]: row[i] for i in range(len(fields))} for row in results
            ]
        else:
            response = [
                dict(zip([col[0] for col in cur.description], row)) for row in results
            ]

        cur.close()
        return jsonify(response)

    except Exception as e:
        return jsonify({"error": f"Database error: {str(e)}"}), 500


@app.route("/plantNode/newPlantNode", methods=["POST"])
def new_plant_node():
    try:
        # Get request data
        data = request.json

        # Validate required fields
        if "macAddress" not in data or "nodeName" not in data:
            return jsonify({"error": "Missing required fields"}), 400

        # Build and execute SQL query
        query = "INSERT INTO automatic_watering_system.plant_nodes (macAddress, nodeName) VALUES (%s, %s)"
        cur = mysql.connection.cursor()
        cur.execute(query, (data["macAddress"], data["nodeName"]))
        mysql.connection.commit()
        cur.close()

        return jsonify({"success": True})

    except Exception as e:
        return jsonify({"error": f"Database error: {str(e)}"}), 500


@app.route("/wifiConfig/retrieveWateringFrequencies", methods=["GET"])
def retrieve_watering_frequencies():
    try:
        cur = mysql.connection.cursor()
        cur.execute(
            "SELECT frequencyID, frequency_name FROM automatic_watering_system.watering_frequencies"
        )
        frequencies = cur.fetchall()
        cur.close()

        return jsonify([{"id": f[0], "name": f[1]} for f in frequencies])

    except Exception as e:
        return jsonify({"error": f"Database error: {str(e)}"}), 500


@app.route("/wifiConfig/retrieveTemperatureThresholds", methods=["GET"])
def retrieve_temperature_thresholds():
    try:
        cur = mysql.connection.cursor()
        cur.execute(
            "SELECT temperatureThresholdID, threshold_name FROM automatic_watering_system.`threshold-temperature`"
        )
        thresholds = cur.fetchall()
        cur.close()

        return jsonify([{"id": t[0], "name": t[1]} for t in thresholds])

    except Exception as e:
        return jsonify({"error": f"Database error: {str(e)}"}), 500


@app.route("/wifiConfig/retrieveMoistureThresholds", methods=["GET"])
def retrieve_moisture_thresholds():
    try:
        cur = mysql.connection.cursor()
        cur.execute(
            "SELECT moistureThresholdID, threshold_name FROM automatic_watering_system.`threshold-soil-moisture`"
        )
        thresholds = cur.fetchall()
        cur.close()

        return jsonify([{"id": t[0], "name": t[1]} for t in thresholds])

    except Exception as e:
        return jsonify({"error": f"Database error: {str(e)}"}), 500


@app.route("/wifiConfig/retrieveLightThresholds", methods=["GET"])
def retrieve_light_thresholds():
    try:
        cur = mysql.connection.cursor()
        cur.execute(
            "SELECT lightThresholdID, threshold_name FROM automatic_watering_system.`threshold-light-sensitivity`"
        )
        thresholds = cur.fetchall()
        cur.close()

        return jsonify([{"id": t[0], "name": t[1]} for t in thresholds])

    except Exception as e:
        return jsonify({"error": f"Database error: {str(e)}"}), 500


@app.route("/wifiConfig/retrieveHumidityThresholds", methods=["GET"])
def retrieve_humidity_thresholds():
    try:
        cur = mysql.connection.cursor()
        cur.execute(
            "SELECT humidityThresholdID, threshold_name FROM automatic_watering_system.`threshold-humidity`"
        )
        thresholds = cur.fetchall()
        cur.close()

        return jsonify([{"id": t[0], "name": t[1]} for t in thresholds])

    except Exception as e:
        return jsonify({"error": f"Database error: {str(e)}"}), 500


# TODO: Create login route
@app.route("/users/login", methods=["OPTIONS", "POST"])
def login():
    if request.method == "OPTIONS":
        response = jsonify({"message": "CORS preflight"})
        response.headers.add(
            "Access-Control-Allow-Origin", request.headers.get("Origin", "*")
        )
        response.headers.add("Access-Control-Allow-Methods", "GET, POST, OPTIONS")
        response.headers.add(
            "Access-Control-Allow-Headers", "Content-Type, Authorization"
        )
        response.headers.add("Access-Control-Allow-Credentials", "true")
        return response, 200  # Make sure OPTIONS returns 200

    try:
        data = request.json
        print("Received login request:", data)

        if not data or "username" not in data or "password" not in data:
            return jsonify({"error": "Missing required fields"}), 400

        query = "SELECT * FROM automatic_watering_system.users WHERE username = %s AND password = %s"
        cur = mysql.connection.cursor()
        cur.execute(query, (data["username"], data["password"]))
        user = cur.fetchone()
        cur.close()

        if user:
            session["username"] = data["username"]
            response = make_response(
                jsonify({"success": True, "message": "Login successful"})
            )

            response.headers.add(
                "Access-Control-Allow-Origin", request.headers.get("Origin", "*")
            )
            response.headers.add("Access-Control-Allow-Credentials", "true")
            return response, 200
        else:
            response = make_response(jsonify({"error": "Invalid credentials"}))

        # Add CORS headers to the response
        response.headers.add(
            "Access-Control-Allow-Origin", request.headers.get("Origin", "*")
        )
        response.headers.add("Access-Control-Allow-Credentials", "true")

        return response, 401

    except Exception as e:
        print("Login error:", str(e))
        error_response = make_response(
            jsonify({"error": "Server error", "details": str(e)})
        )
        error_response.headers.add(
            "Access-Control-Allow-Origin", request.headers.get("Origin", "*")
        )
        error_response.headers.add("Access-Control-Allow-Credentials", "true")
        return error_response, 500


@app.route("/users/verify", methods=["GET"])
def verify_user():
    try:
        username = session.get("username")
        logging.info(f"Verification attempt - Session present: {bool(username)}")
        if not username:
            return (
                jsonify(
                    {
                        "error": "User not verified",
                        "message": "No username in session",
                    }
                ),
                401,
            )

        response = jsonify(
            {
                "success": True,
                "message": "User verified",
                "username": username,
            }
        )

        # Add CORS headers explicitly
        response.headers.add("Access-Control-Allow-Credentials", "true")
        response.headers.add(
            "Access-Control-Allow-Origin", request.headers.get("Origin")
        )

        return response, 200

    except Exception as e:
        logging.error(f"Verification error: {str(e)}", exc_info=True)
        return (
            jsonify(
                {
                    "error": "Server error",
                    "message": "An error occurred during verification",
                }
            ),
            500,
        )


@app.route("/users/signup", methods=["POST"])
def create_user():

    # Get request data
    data = request.json

    # Build and execute SQL query
    username = data.get("username")
    password = data.get("password")
    email = data.get("email")
    phone_number = data.get("phoneNumber")
    first_name = data.get("firstName")
    last_name = data.get("lastName")
    preferred_temperature_unit = data.get("preferredTemperatureUnit")

    query = """INSERT INTO automatic_watering_system.users 
                (username, password, email, phoneNumber, firstName, lastName, preferredTemperatureUnit) 
                VALUES (%s, %s, %s, %s, %s, %s, %s)"""

    try:
        cur = mysql.connection.cursor()
        cur.execute(
            query,
            (
                username,
                password,
                email,
                phone_number,
                first_name,
                last_name,
                preferred_temperature_unit,
            ),
        )
        mysql.connection.commit()
        cur.close()
        return jsonify({"message": "User added successfully"}), 201
    except Exception as e:
        return jsonify({"error": str(e)}), 500


if __name__ == "__main__":
    app.run(debug=True)
