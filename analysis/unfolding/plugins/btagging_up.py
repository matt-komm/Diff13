from defaultModules import ResponseMatrix

class ResponseMatrix_btagging_up(ResponseMatrix):
    def __init__(self,options=[]):
        ResponseMatrix.__init__(self,options)
        
    @staticmethod
    def get():
        print "derived class",__file__
