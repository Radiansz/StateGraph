#ifndef STATE_H
#define STATE_H
#include <vector>
#include <StateGraph.h>

#include <QString>
#include "QDomDocument"
#include "QDomNode"
#include "QDomElement"
#include "QDomText"
#include "badxmlexception.h"
namespace States {

    template <class V>
    class StateDiagram;

    template <class T>
    class State
    {
        friend class StateDiagram<T>;
    protected:

        T data;
        bool marked;

        void swap(State<T> & other){
            std::swap(data,other.data);
        }

    public:

        State(){
            marked = false;
        }



        State(State<T> &state){
            this->data = state.data;
            marked = false;
        }

        State(T data){
            this->data = data;
            marked = false;
        }

        virtual ~State(){

        }


        T getData(){
            return data;
        }
        void setData(T data){
            this->data = data;
        }


        virtual QString toString(){
            return "State";
        }


     //   Iterator getConnectedNodes();
        State<T>& operator = (State<T> other){
            swap(other);
            return *this;
        }



        virtual QString serialize(){
            QDomNode el = serializeToDom();
            QDomDocument doc = el.toDocument();
            return doc.toString() ;

        }

        virtual QDomNode serializeToDom(){
            QString temp;
            QDomDocument doc("");
            QDomElement head = doc.createElement("State");
            QDomElement node1 = doc.createElement("data");
            QDomNode elem = data.serializeToDom();
            node1.appendChild(elem);
            head.appendChild(node1);
            doc.appendChild(head);
            return doc;
        }

        virtual void deserialize(QString data){
            QDomDocument doc;
            doc.setContent(data);
            deserializeFromDom(doc);
        }

        virtual void deserializeFromDom(QDomNode node){

            QDomNode head = node;
            QDomNode temp;
            if(head.nodeName() == "State" ){
                temp = head.firstChild();
                if( temp.nodeName() == "data"){
                    T dataN;
                    dataN.deserializeFromDom(temp.firstChild());
                    data = dataN;
                }
                else
                    throw BadXMLException();
            }
            else
                throw BadXMLException();
        }

    };
}
#endif // STATE_H
