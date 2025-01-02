from flask import Flask, jsonify, request, make_response, session  # type: ignore
import secrets
from flask_mysqldb import MySQL  # type: ignore
import os
from flask_cors import CORS  # type: ignore
import logging
import re
from datetime import timedelta, datetime
from functools import wraps

# Set up logging
logging.basicConfig(
    level=logging.INFO, format="%(asctime)s [%(levelname)s] %(message)s"
)


app = Flask(__name__)
app.secret_key = os.environ.get("SECRET_KEY", "dev-key-for-local-testing")
app.permanent_session_lifetime = timedelta(days=7)  # Set session lifetime to 7 days

# Add this near the top of app.py with other configurations
app.config["SESSION_COOKIE_SECURE"] = True
app.config["SESSION_COOKIE_SAMESITE"] = "None"  # Important for cross-origin
app.config["SESSION_COOKIE_DOMAIN"] = (
    "automatic-watering-system-api-e673f34a5955.herokuapp.com"
)


# Login decorator for protected routes
def login_required(f):
    @wraps(f)
    def decorated_function(*args, **kwargs):
        if "username" not in session:
            return jsonify({"error": "Authentication required"}), 401
        return f(*args, **kwargs)

    return decorated_function


# Function to validate origin with regex
def allowed_origin(origin):
    if not origin:
        return False

    allowed_patterns = [
        r"^http://192\.168\.1\.240(:\d+)?$",
        r"^http://\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}(:\d+)?$",
        r"^http://localhost(:\d+)?$",
    ]

    return any(re.match(pattern, origin) for pattern in allowed_patterns)


# CORS configuration
CORS(
    app,
    resources={
        r"/*": {
            "origins": "*",  # Be explicit
            "methods": ["GET", "POST", "OPTIONS"],
            "allow_headers": [
                "Content-Type",
                "Authorization",
                "Access-Control-Allow-Credentials",
            ],
            "expose_headers": ["Content-Type", "Authorization", "Set-Cookie"],
            "supports_credentials": True,
        }
    },
)

# MySQL configurations
app.config["MYSQL_HOST"] = os.environ.get("MYSQL_HOST")
app.config["MYSQL_USER"] = os.environ.get("MYSQL_USER")
app.config["MYSQL_PASSWORD"] = os.environ.get("MYSQL_PASSWORD")
app.config["MYSQL_DB"] = os.environ.get("MYSQL_DB")

mysql = MySQL(app)


@app.after_request
def after_request(response):
    origin = request.headers.get("Origin")

    if origin and allowed_origin(origin):
        response.headers["Access-Control-Allow-Origin"] = origin
        response.headers["Access-Control-Allow-Credentials"] = "true"
        response.headers["Access-Control-Allow-Headers"] = (
            "Content-Type, Authorization, Access-Control-Allow-Credentials"
        )
        response.headers["Access-Control-Allow-Methods"] = "GET, POST, OPTIONS"

    return response


@app.route("/")
def index():
    try:
        cur = mysql.connection.cursor()
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
@login_required
def new_plant_node():
    try:
        # Get request data
        data = request.json

        # Retrieve username from session
        username = session.get("username")

        # Extract data from the request
        node_name = data.get("nodeName")
        plant_species = data.get("plantSpecies")
        watering_frequency_id = data.get("wateringFrequencyID")
        light_threshold_id = data.get("lightThresholdID")
        humidity_threshold_id = data.get("humidityThresholdID")
        moisture_threshold_id = data.get("moistureThresholdID")
        temperature_threshold_id = data.get("temperatureThresholdID")

        # Retrieve userUUID from the database
        user_query = """
            SELECT userUUID FROM automatic_watering_system.users WHERE username = `%s`
        """
        logging.info(f"userUUID Query: {user_query % username}")
        cur = mysql.connection.cursor()
        cur.execute(user_query, (username,))
        user = cur.fetchall()
        cur.close()

        if not user:
            cur.close()
            return jsonify({"error": "User not found"}), 404

        user_uuid = user["userUUID"]
        logging.info(f"User UUID: {user}")
        # Check for duplicate plant node
        check_query = """
            SELECT * FROM plant_nodes
            WHERE connectedUserUUID = %s AND nodeName = %s
        """

        logging.info(f"Check Query: {check_query % user_uuid, node_name}")
        cur = mysql.connection.cursor()
        cur.execute(check_query, (user_uuid, node_name))
        existing_plant = cur.fetchone()
        cur.close()

        if existing_plant:
            cur.close()
            return jsonify({"error": "Duplicate plant node found"}), 409

        # Insert the new plant node into the database
        query = """
            INSERT INTO plant_nodes (
                connectedUserUUID, nodeName, plantSpecies, wateringFrequencyID,
                lightThresholdID, humidityThresholdID, moistureThresholdID,
                temperatureThresholdID
            ) VALUES (%s, %s, %s, %s, %s, %s, %s, %s)
        """
        params = (
            user_uuid,
            node_name,
            plant_species,
            watering_frequency_id,
            light_threshold_id,
            humidity_threshold_id,
            moisture_threshold_id,
            temperature_threshold_id,
        )
        logging.info(f"Final Query: {query % params}")

        cur.execute(query, params)
        mysql.connection.commit()
        cur.close()

        return jsonify({"message": "Plant node registered successfully!"}), 201

    except Exception as e:
        return jsonify({"error": f"Database error: {str(e)}"}), 500


@app.route("/wifiConfig/retrieveWateringFrequencies", methods=["GET"])
def retrieve_watering_frequencies():
    try:
        cur = mysql.connection.cursor()
        cur.execute(
            "SELECT wateringFrequencyID, frequency_name FROM automatic_watering_system.watering_frequencies"
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


@app.route("/users/login", methods=["OPTIONS", "GET", "POST"])
def login():
    logging.info(f"Session before: {session}")  # Add this
    logging.info(f"Request cookies: {request.cookies}")  # Add this

    if request.method == "OPTIONS":
        response = make_response()
        origin = request.headers.get("Origin")
        logging.info(f"OPTIONS request from origin: {origin}")

        if origin and allowed_origin(origin):
            response.headers["Access-Control-Allow-Origin"] = origin
            response.headers["Access-Control-Allow-Methods"] = "GET, POST, OPTIONS"
            response.headers["Access-Control-Allow-Headers"] = (
                "Content-Type, Authorization, Access-Control-Allow-Credentials"
            )
            response.headers["Access-Control-Allow-Credentials"] = "true"
            logging.info("CORS headers set for OPTIONS request")

        return response

    try:
        if request.method == "POST":
            data = request.json
            logging.info(f"Received login request: {data}")

            if not data or "username" not in data or "password" not in data:
                logging.info("Missing required fields in login request")
                return jsonify({"error": "Missing required fields"}), 400

            query = "SELECT * FROM automatic_watering_system.users WHERE username = %s AND password = %s"
            cur = mysql.connection.cursor()
            cur.execute(query, (data["username"], data["password"]))
            user = cur.fetchone()
            cur.close()

            if user:
                if data["rememberMe"]:
                    session.permanent = True
                    session["username"] = data["username"]
                    session["last_activity"] = datetime.now().isoformat()
                    logging.info(f"User {data['username']} logged in successfully")
                    logging.info(f"Session after login: {session}")  # Add this

                return (
                    jsonify(
                        {
                            "success": True,
                            "message": "Login successful",
                            "username": data["username"],
                        }
                    ),
                    200,
                )

            logging.info(f"Invalid credentials for user {data['username']}")
            return jsonify({"error": "Invalid credentials"}), 401

        else:  # GET request - verify session
            username = session.get("username")
            last_activity = session.get("last_activity")

            if not username:
                logging.info("No username in session for verification")
                return jsonify({"error": "Not authenticated"}), 401

            # Check session age
            if last_activity:
                last_activity_time = datetime.fromisoformat(last_activity)
                if datetime.now() - last_activity_time > timedelta(days=7):
                    session.clear()
                    logging.info("Session expired for user")
                    return jsonify({"error": "Session expired"}), 401

            # Verify user exists in database
            cur = mysql.connection.cursor()
            cur.execute(
                "SELECT username FROM automatic_watering_system.users WHERE username = %s",
                (username,),
            )
            user = cur.fetchone()
            cur.close()

            if user:
                session["last_activity"] = (
                    datetime.now().isoformat()
                )  # Update last activity
                logging.info(f"User {username} verified successfully")
                return (
                    jsonify(
                        {
                            "success": True,
                            "message": "User verified",
                            "username": username,
                        }
                    ),
                    200,
                )

            logging.info(f"Invalid session for user {username}")
            return jsonify({"error": "Invalid session"}), 401

    except Exception as e:
        logging.error(f"Login error: {str(e)}", exc_info=True)
        return jsonify({"error": "Server error"}), 500


@app.route("/users/logout", methods=["POST"])
def logout():
    session.clear()
    return jsonify({"success": True, "message": "Logged out successfully"}), 200


@app.route("/users/signup", methods=["POST"])
def create_user():
    data = request.json

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
