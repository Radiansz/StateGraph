#ifndef SUPERSTATE_H
#define SUPERSTATE_H

#include <state.h>
#include <stategraph.h>
#include <QString>
#include "QDomDocument"
#include "QDomNode"
#include "QDomElement"
#include "QDomText"

namespace States {

template <class V>
class StateGraph;

template <class C>
class State;

template <class T>
class SuperState : public State<T>
{

    StateGraph<T> * innerGraph;

public:

    void setInnerGraph(StateGraph<T>* graph){
        innerGraph = graph;
    }

    StateGraph<T>* getInnerGraph(){
        return innerGraph;
    }

    virtual QString toString(){
        return "SuperState";
    }

    QString serialize(){
        QDomNode el = serializeToDom();
        QDomDocument doc = el.toDocument();
        return doc.toString() ;

    }

    QDomNode serializeToDom(){
        QDomDocument doc = State<T>::serializeToDom().toDocument();
        doc.firstChildElement().setTagName("SuperState");
        QDomElement node2 = doc.createElement("Graph");
        QDomNode graphNode;
        if(innerGraph!= 0){
            graphNode = innerGraph->serializeToDom();
            node2.appendChild(graphNode);
        }
        else
            node2.appendChild(doc.createTextNode("0"));
        doc.firstChildElement().appendChild(node2);
        return doc;
    }

    SuperState(){
        innerGraph = 0;
    }

    SuperState(T data): State<T>(data){
        innerGraph = 0;
    }

    SuperState(SuperState<T> &ss): State<T>(ss){
        innerGraph = 0;

    }

    SuperState<T>& operator = (State<T> other){
        swap(other);
        return *this;
    }

    ~SuperState();


    void deserialize(QString data){
        QDomDocument doc;
        doc.setContent(data);
        deserializeFromDom(doc);
    }

    void deserializeFromDom(QDomNode node){
        QDomNode head = node;
        QDomNode temp;
        if(head.nodeName() == "SuperState" ){
            temp = head.firstChild();
            if( temp.nodeName() == "data"){
                T dataN;
                dataN.deserializeFromDom(temp.firstChild());
                State<T>::setData(dataN);
            }
            temp = temp.nextSiblingElement();
            if( temp.nodeName() == "Graph"){
                QDomNode node = temp.firstChild();
                if(node.isText()){
                    innerGraph == NULL;
                }
                else
                {
                    StateGraph<T>* graph = new StateGraph<T>();
                    graph->deserializeFromDom(node);
                    innerGraph = graph;
                }
            }
        }
    }

};
}
#endif // SUPERSTATE_H
