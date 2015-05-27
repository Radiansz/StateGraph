#ifndef STATEGRAPH_H
#define STATEGRAPH_H

#include <vector>
#include <state.h>
#include <superstate.h>
#include <iterator>
#include <stateiterator.h>
#include <stateexception.h>
#include <unknownstateexception.h>
#include <wrongindexstateexception.h>
#include "QDomDocument"
#include "QDomNode"
#include "QDomElement"
#include "QDomText"
#include "QFile"
#include "QTextStream"
namespace States {
    template <class V>
    class State;

    template <class V>
    class SuperState;

    template <class D>
    class StateIterator;

    template <class T>
    class StateGraph
    {


        State<T>* initialS;
        State<T>* finalS;
        std::vector< State<T>* > states;
        std::vector< std::vector<int> > connections;
        int statesAmount;

        State<T>* src;
        State<T>* dst;
        std::vector< State<T>* > accum;

        void restoreConnections(int edge){
            for(int i=0; i<connections.size(); i++){
                int j=0,delInd=-1;
                for(std::vector<int>::iterator itr = connections[i].begin(); itr < connections[i].end(); itr++ ){
                    if(*itr == edge){
                        delInd = j;
                        continue;
                    }
                    if(*itr > edge)
                        connections[i][j] = *itr - 1;
                    j++;
                }
                if(delInd != -1)
                    connections[i].erase(connections[i].begin() + delInd);
            }
        }


        int findIndex(State<T>* state){
            for(int i=0; i< states.size(); i++){
                if(state==states[i])
                    return i;
            }
            throw UnknownStateException();
            return -1;
        }

        State<T>* findState(int index){
            if(index < statesAmount)
                return states[index];
            else
                throw WrongIndexStateException();
            return 0;
        }

        bool tracing(int state){
            State<T>* stt = findState(state);
            State<T>* tempS;
            bool res = false;
            stt->marked = true;
            for(std::vector<int>::iterator i = connections[state].begin(); i < connections[state].end(); ++i){
                tempS = findState(*i);
                if(tempS == dst){
                    accum.push_back(tempS);
                    res = true;
                    break;
                }
                if(tempS->marked)
                    continue;
                else{
                    if(tracing(*i)){
                        accum.push_back(tempS);
                        res = true;
                        break;
                    }
                }
            }
            stt->marked = false;
            return res;

        }

        QDomNode connectionDom(int source, int destination){
            QString temp;
            QDomDocument doc("");
            QDomElement conn = doc.createElement("Connection");
            QDomElement srcDom = doc.createElement("Source");
            QDomElement dstDom = doc.createElement("Destination");
            srcDom.appendChild(doc.createTextNode(temp.setNum(source)));
            dstDom.appendChild(doc.createTextNode(temp.setNum(destination)));
            conn.appendChild(srcDom);
            conn.appendChild(dstDom);
            doc.appendChild(conn);
            return doc;

        }

        State<T>* newStateGen(QDomNode node){
            QDomNode temp;
            if(node.nodeName() == "State"){
                State<T>* s = new State<T>();
                s->deserializeFromDom(node);
                return s;
            }
            if(node.nodeName() == "SuperState"){
                State<T>* s = new SuperState<T>();
                s->deserializeFromDom(node);
                return s;
            }
            return NULL;
        }

    public:
        typedef StateIterator<T> iterator ;
        void setInitialState(State<T> *newState)
        {
            if(findIndex(newState) == -1 )
                addState(newState);
            initialS = newState;
        }

        void setFinalState(State<T> *newState){
            if(findIndex(newState) == -1 )
                addState(newState);
            finalS = newState;
        }

        bool isInitialState(State<T>* state){
            return *state==initialS ? true : false;
        }

        bool isFinalState(State<T>* state){
            return *state==finalS ? true : false;
        }

        void addState(State<T>* newState){
            states.push_back(newState);
            connections.push_back(*(new std::vector<int>()));
            statesAmount++;
        }


        void addConnection(State<T>* source, State<T>* destination){
            addConnection(findIndex(source),findIndex(destination));
        }

        void addConnection(int srcIndex, int dstIndex){
            if(srcIndex>=0 && srcIndex<statesAmount && dstIndex>=0 && dstIndex<statesAmount)
                connections[srcIndex].push_back(dstIndex);
            else
                throw WrongIndexStateException();
        }



        void removeState(State<T>* state){
            removeState(findIndex(state));
        }

        void removeState(int stateIndex){
            if(stateIndex>=0 && stateIndex <statesAmount){
                states.erase(states.begin() + stateIndex);
                connections.erase(connections.begin() + stateIndex);
                restoreConnections(stateIndex);
                statesAmount--;
            }
            else
                throw WrongIndexStateException();

        }

        void removeConnection(State<T>* source, State<T>* destination){
            removeConnection(findIndex(source),findIndex(destination));
        }

        void removeConnection(int source, int destination){
            if(!(source>=0 && source<statesAmount && destination>=0 && destination<statesAmount))
                throw WrongIndexStateException();
            int toDel = -1;
            for(int i = 0 ;  i < connections[source].size(); i++){
                if(connections[source][i] == destination){
                    toDel = i;
                    break;
                }
            }
            if(toDel != -1)
                connections[source].erase(connections[source].begin() + toDel);
        }

        iterator begin(){
            StateIterator<T> iter(states);
            return iter;

        }


        iterator getConnections(State<T> *state){
            return getConnections(findIndex(state));
        }

        iterator getConnections(int stateIndex){
            std::vector< State<T>* > data;
            for(int i = 0 ;  i < connections[stateIndex].size(); i++){
                data.push_back(findState(connections[stateIndex][i]));
            }
            return data;
        }
        iterator findPath(){
            return findPath(initialS, finalS);
        }

        iterator findPath(State<T>* source, State<T>* destination){
            std::vector<State<T>* > path;
            src = source;
            dst = destination;
            StateIterator<int>* itr;
            int srcI = findIndex(src), dstI = findIndex(dst);
            if(!(srcI>=0 && srcI<statesAmount && dstI>=0 && dstI<statesAmount))
                return path;
            path.clear();
            if(tracing(srcI)){
                accum.push_back(src);
                for(int i = accum.size() - 1; i>=0; i--)
                    path.push_back(accum[i]);
            }
            StateIterator<T> iter(path);
            return iter;
        }



        StateGraph(){
            statesAmount = 0;
        }
        ~StateGraph(){

        }
        void serializeToFile(QString file){
            QFile* inFile = new QFile(file);
            if(inFile->open(QIODevice::WriteOnly)){
                QTextStream out(inFile);
                out << serialize();
                inFile->close();
            }
        }

        QString serialize(){
            QDomNode el = serializeToDom();
            QDomDocument doc = el.toDocument();
            return doc.toString() ;
        }

        QDomNode serializeToDom(){
            QString temp;
            QDomDocument doc("");
            QDomElement head = doc.createElement("StateGraph");
            QDomElement statesTag = doc.createElement("States");
            for(int i = 0; i < states.size(); i++){
                QDomNode newState = states[i]->serializeToDom();
                statesTag.appendChild(newState);
            }

            QDomElement connTag = doc.createElement("Connections");
            for(int i = 0; i < connections.size(); i++){
                if(connections[i].size() > 0){
                    for(int j = 0; j < connections[i].size(); j++){
                        connTag.appendChild(connectionDom(i, connections[i][j]));
                    }
                }
            }
            QDomElement initTag = doc.createElement("initialState");
            QDomElement finalTag = doc.createElement("finalState");
            int tempIn;
            try{
                tempIn = findIndex(initialS);
            }
            catch(WrongIndexStateException e){
                tempIn = - 1;
            }
            initTag.appendChild(doc.createTextNode(temp.setNum(tempIn)));
            try{
                tempIn = findIndex(finalS);
            }
            catch(WrongIndexStateException e){
                tempIn = - 1;
            }
            finalTag.appendChild(doc.createTextNode(temp.setNum(tempIn)));
            head.appendChild(statesTag);
            head.appendChild(connTag);
            head.appendChild(initTag);
            head.appendChild(finalTag);
            doc.appendChild(head);
            return doc;
        }
        void deserializeFromFile(QString file){
            QFile* inFile = new QFile(file);
            QString data;
            if(inFile->open(QIODevice::ReadOnly)){
                QTextStream in(inFile);
                data = in.readAll();
                inFile->close();
                deserialize(data);
            }
        }

        void deserialize(QString data){
            statesAmount = 0;
            states.clear();
            connections.clear();
            QDomDocument doc;
            doc.setContent(data);
            deserializeFromDom(doc.firstChild());
        }

        void deserializeFromDom(QDomNode node){
            statesAmount = 0;
            states.clear();
            connections.clear();
            QDomNode head = node;
            QDomNode temp;
            if(head.nodeName() == "StateGraph"){
                temp = head.firstChild();
                if(temp.nodeName() == "States"){
                    QDomNodeList l = temp.childNodes();
                    for(int i = 0; i<l.length();i++){
                        QDomNode node = l.at(i);
                        State<T>* tempS = newStateGen(node);
                        if(tempS)
                            addState(tempS);
                    }
                }
                temp = temp.nextSiblingElement();
                if(temp.nodeName() == "Connections"){
                    QDomNodeList l = temp.childNodes();
                    for(int i = 0; i<l.length();i++){
                        QDomNode node = l.at(i);
                        if(node.nodeName() == "Connection"){
                            QDomNode tempSrc = node.firstChild();
                            QDomNode tempDst = node.lastChild();
                            if(tempSrc.nodeName() == "Source" && tempDst.nodeName() == "Destination" ){
                                int t1 = tempSrc.firstChild().toText().data().toInt();
                                int t2 = tempDst.firstChild().toText().data().toInt();
                                connections[t1].push_back(t2);
                            }
                        }
                    }
                }
                temp = temp.nextSiblingElement();
                if( temp.nodeName() == "initialState"){
                    int t1 = temp.firstChild().toText().data().toInt();
                    if(t1 >= 0 && t1 < statesAmount)
                        initialS = findState(t1);
                }
                temp = temp.nextSiblingElement();
                if( temp.nodeName() == "finalState"){
                    int t1 = temp.firstChild().toText().data().toInt();
                    if(t1 >= 0 && t1 < statesAmount)
                        finalS = findState(t1);
                }
            }
        }
    };
}
#endif // STATEGRAPH_H
