import das_client as das
import time
import sys
import csv


def intented(msg,level=0):
    ret =""
    for i in range(level):
        ret+= "  "
    ret+= str(msg)+"\r\n"
    return ret

def showJSON(data,level=0):
    ret=""
    if type(data)==type(dict()):
        for key in data.keys():
            ret+=intented("<key "+key+">",level)
            ret+=showJSON(data[key],level+1)
    elif type(data)==type(list()):
        for i in range(len(data)):
            ret+=intented("<list "+str(i)+">",level)
            ret+=showJSON(data[i],level+1)
    else:
        ret+=intented(data,level+1)
    return ret
    
def do_query(query):
    for retry in range(10):
        try:
            data = das.get_data(
                "https://cmsweb.cern.ch", 
                query, 0, 0, 0, 90, "", "")
            if data["status"]!="ok":
                raise Exception("Status not ok! "+str(data["status"]))
            return data
        except Exception,e:
            logging.warning(str(e)+" -> wait for "+str(10+2*retry)+"sec before retry...")
        time.sleep(10+2*retry)
    logging.error("too many retries -> exit!")
    raise Exception("too many retries")

                  

data = do_query("run,lumi,file dataset=/SingleMuon/Run2015D-PromptReco-v3/MINIAOD instance=prod/global")["data"]
runLumiFileDict={}
files=[]
nLumi=0

for infoBlock in data:
    run = infoBlock["run"][0]["run_number"]
    lumiRange = infoBlock["lumi"][0]["number"]
    fileName = infoBlock["file"][0]["name"]
    #print "run: ",run,", lumi: ",lumiRange,", file: ",fileName
    
    if not runLumiFileDict.has_key(run):
        runLumiFileDict[run]={}
    for lumiInfo in lumiRange:
        nLumi+=lumiInfo[1]-lumiInfo[0]+1
        for lumi in range(lumiInfo[0],lumiInfo[1]+1):
        
            if not runLumiFileDict[run].has_key(lumi):
                runLumiFileDict[run][lumi]=fileName
                
    if fileName not in files:
        files.append(fileName)            
    
print "N(runs)=",len(runLumiFileDict.keys()),", N(lumi)=",nLumi,", N(files)=",len(files)





