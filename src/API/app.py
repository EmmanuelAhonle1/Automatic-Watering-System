from flask import Flask, jsonify, request  # type: ignore
from flask_mysqldb import MySQL  # type: ignore
import os
from flask_cors import CORS  # type: ignore

app = Flask(__name__)
# CORS(app)

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


@app.route("/plantNode/insert", methods=["POST"])
def insert_plant():
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


if __name__ == "__main__":
    app.run(debug=True)
