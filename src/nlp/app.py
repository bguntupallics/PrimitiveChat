from fastapi import FastAPI
from pydantic import BaseModel
from sentence_transformers import SentenceTransformer, util
import torch

# Initialize the FastAPI application
app = FastAPI()

# Load the pre-trained model once at startup
model = SentenceTransformer('all-MiniLM-L6-v2')

# Define the keywords and pre-encode them
keywords = ["exit", "help"]
keyword_embeddings = model.encode(keywords, convert_to_tensor=True)


# Pydantic model for request body
class TextInput(BaseModel):
    text: str


@app.post("/compare")
def compare_text(input_data: TextInput):
    """
    Endpoint that takes a text input and returns which keyword it's closer to.
    """
    sentence = input_data.text  # Extract the user input from the JSON body

    # Encode the user's sentence to get an embedding
    sentence_embedding = model.encode(sentence, convert_to_tensor=True)

    # Compute cosine similarity between the sentence and each keyword
    cosine_scores = util.cos_sim(sentence_embedding, keyword_embeddings)

    # Find the keyword with the highest similarity
    max_index = torch.argmax(cosine_scores).item()
    closest_keyword = keywords[max_index]

    return {
        "input_sentence": sentence,
        "scores": cosine_scores.tolist(),
        "closest_keyword": closest_keyword
    }
