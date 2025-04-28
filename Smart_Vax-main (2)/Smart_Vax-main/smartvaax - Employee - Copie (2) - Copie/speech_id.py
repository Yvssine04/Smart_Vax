import sys
import speech_recognition as sr

r = sr.Recognizer()
mic = sr.Microphone()

def recognize_id():
    with mic as source:
        print("LISTENING", flush=True)
        r.adjust_for_ambient_noise(source)
        try:
            audio = r.listen(source, timeout=5)
            text = r.recognize_google(audio, language="fr-FR").upper()
            # Keep only letters/numbers
            clean_text = "".join(c for c in text if c.isalnum())
            print(clean_text, flush=True)
        except sr.WaitTimeoutError:
            print("TIMEOUT", flush=True)
        except Exception as e:
            print("ERROR:" + str(e), flush=True)

if __name__ == "__main__":
    recognize_id()