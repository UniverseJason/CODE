class myDatabase:
    def __init__(self, question, answer, mutiSelectAns=[]):
        self.question = question
        self.answer = answer
        self.mutiSelectAns = mutiSelectAns
    
    def getQuestion(self) -> str:
        return self.question

    def getAnswer(self) -> str:
        return self.answer

    def getMutiSelectAns(self) -> list:
        return self.mutiSelectAns