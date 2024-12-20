from flask import Flask, jsonify # type: ignore
from flask_mysqldb import MySQL # type: ignore
import os

app = Flask(__name__)

# MySQL configurations
app.config['MYSQL_HOST'] = os.getenv('MYSQL_HOST')
app.config['MYSQL_USER'] = os.getenv('MYSQL_USER')
app.config['MYSQL_PASSWORD'] = os.getenv('MYSQL_PASSWORD')
app.config['MYSQL_DB'] = os.getenv('MYSQL_DB')

mysql = MySQL(app)

@app.route('/')
def index():
    return "Welcome to the Automatic Watering System API"

@app.route('/plantNode/<mac_address>', methods=['GET'])
def get_plant(mac_address):
    cur = mysql.connection.cursor()
    cur.execute("SELECT * FROM automatic_watering_system.plantNodes WHERE mac_address = %s", (mac_address,))
    plant = cur.fetchone()
    cur.close()
    if plant:
        return jsonify({
            'mac_address': plant[0],
            'name': plant[1],
            'type': plant[2],
            'last_watered': plant[3]
        })
    else:
        return jsonify({'error': 'Plant not found'}), 404

if __name__ == '__main__':
    app.run(debug=True)
