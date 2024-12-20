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

@app.route('/plantNode/<mac_address>', methods=['GET'])
def get_plant(mac_address):
    cur = None
    try:
        # Use DictCursor instead of regular cursor
        cur = mysql.connection.cursor(dictionary=True)
        
        query = """
            SELECT macAddress, nodeName 
            FROM automatic_watering_system.plant_nodes 
            WHERE macAddress = %s
        """
        cur.execute(query, (mac_address,))
        plant = cur.fetchone()
        
        if plant:
            result = {
                'mac_address': plant['macAddress'],  # Access by column name
                'name': plant['nodeName']           # Access by column name
            }
            mysql.connection.commit()
            return jsonify(result)
        else:
            return jsonify({'error': 'Plant not found'}), 404
            
    except mysql.connector.Error as db_err:
        app.logger.error(f"Database error: {db_err}")
        return jsonify({'error': 'Database connection error'}), 500
    except Exception as e:
        app.logger.error(f"Server error: {e}")
        return jsonify({'error': 'Internal server error'}), 500
    finally:
        if cur:
            cur.close()

if __name__ == '__main__':
    app.run(debug=True)