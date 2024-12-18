from flask import Flask, jsonify # type: ignore
from flask_mysqldb import MySQL # type: ignore

app = Flask(__name__)

# MySQL configurations
app.config['MYSQL_HOST'] = 'your_mysql_host'
app.config['MYSQL_USER'] = 'your_mysql_user'
app.config['MYSQL_PASSWORD'] = 'your_mysql_password'
app.config['MYSQL_DB'] = 'your_database_name'

mysql = MySQL(app)

@app.route('/')
def index():
    return "Welcome to the Automatic Watering System API"

@app.route('/plantNode/<mac_address>', methods=['GET'])
def get_plant(mac_address):
    cur = mysql.connection.cursor()
    cur.execute("SELECT * FROM plants WHERE mac_address = %s", (mac_address,))
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
