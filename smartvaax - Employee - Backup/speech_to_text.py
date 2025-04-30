import speech_recognition as sr

recognizer = sr.Recognizer()
microphone = sr.Microphone()

def recognize_speech():
    with microphone as source:
        recognizer.adjust_for_ambient_noise(source)
        try:
            audio = recognizer.listen(source, timeout=3)
            text = recognizer.recognize_google(audio, language="fr-FR")
            print(text)  # ONLY print the result
        except sr.WaitTimeoutError:
            print("TimeoutError")
        except sr.UnknownValueError:
            print("UnknownValue")
        except sr.RequestError:
            print("RequestError")

recognize_speech()
