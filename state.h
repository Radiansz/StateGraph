#ifndef STATE_H
#define STATE_H
#include <vector>
#include <stategraph.h>

#include <QString>
#include "QDomDocument"
#include "QDomNode"
#include "QDomElement"
#include "QDomText"
namespace States {

    template <class V>
    class StateGraph;

    template <class T>
    class State
    {
        friend class StateGraph<T>;
    protected:

        T data;
        bool marked;

        void swap(State<T> & other){
            std::swap(data,other.data);
        }

    public:


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

        ~State(){

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
            }
        }

    };
}
#endif // STATE_H
