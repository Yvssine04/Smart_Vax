import speech_to_text as sr

recognizer = sr.Recognizer()
microphone = sr.Microphone()

def recognize_speech():
    with microphone as source:
       
        recognizer.adjust_for_ambient_noise(source)  # Adjust for ambient noise
        try:
            audio = recognizer.listen(source, timeout=3)  # Listen for up to 10 seconds
            
            text = recognizer.recognize_google(audio, language="fr-FR")
            print(f" {text}")
        except sr.WaitTimeoutError:
            print("Listening timed out. Please try again.")
        except sr.UnknownValueError:
            print("Sorry, I could not understand the audio.")
        except sr.RequestError as e:
            print(f"Could not request results from Google Speech Recognition service; {e}")

recognize_speech()
