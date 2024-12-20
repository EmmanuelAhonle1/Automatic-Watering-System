from flask import Flask, jsonify # type: ignore
from flask_mysqldb import MySQL # type: ignore
import os

app = Flask(__name__)

# MySQL configurations - using your provided credentials
# (but please change these ASAP for security!)
app.config['MYSQL_HOST'] = "automatic-watering-system.cdq4ye8ggh8i.us-east-1.rds.amazonaws.com"
app.config['MYSQL_USER'] = "admin"
app.config['MYSQL_PASSWORD'] = "Mighty_Duck1738"
app.config['MYSQL_DB'] = "automatic_watering_system"  # Make sure this is your actual database name

mysql = MySQL(app)

@app.route('/')
def index():
    try:
        cur = mysql.connection.cursor()
        # Test the connection
        cur.execute('SELECT 1')
        cur.close()
        return "Database connection successful! Welcome to the Automatic Watering System API"
    except Exception as e:
        return jsonify({'error': f'Database connection failed: {str(e)}'}), 500

@app.route('/plantNode/<mac_address>', methods=['GET'])
def get_plant(mac_address):
    try:
        cur = mysql.connection.cursor()
        # Remove the database name from the query since it's specified in MYSQL_DB config
        cur.execute("SELECT macAddress, nodeName FROM automatic_watering_system.plant_nodes WHERE macAddress = %s", (mac_address,))
        plant = cur.fetchone()
        cur.close()
        
        if plant:
            return jsonify({
                'mac_address': plant[0],
                'name': plant[1]
            })
        else:
            return jsonify({'error': 'Plant not found'}), 404
            
    except Exception as e:
        return jsonify({'error': f'Database error: {str(e)}'}), 500

if __name__ == '__main__':
    app.run(debug=True)