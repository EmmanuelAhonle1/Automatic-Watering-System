# dataAPI.py

from fastapi import FastAPI
import os
from dotenv import load_dotenv  # Ensure this import is present

load_dotenv()  # This should be called before accessing environment variables

app = FastAPI()

@app.get("/")
def read_root():
    val = os.environ.get('MY_PASSWORD')  # Check if MY_PASSWORD is retrieved
    return {"Hello": int(val)}  # This should return your password

@app.get("/items/{item_id}")
def read_item(item_id: int, q: str = None):
    return {"item_id": item_id, "query": q}
