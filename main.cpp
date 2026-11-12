#include <iostream>
#include <string>
#include <stack>
#include <fstream>
using namespace std;

struct Node{
    Node* prev;
    Node* next;
    string url;
};


class BrowserHistory{
    private:
        stack<string> s;
        int size=1;
        int capacity=50;
        Node* head;
        Node* current;
    public:
        BrowserHistory(string homepage){
            head = new Node{nullptr, nullptr, homepage};
            current = head;
        }
        BrowserHistory(const BrowserHistory&) = delete;
        BrowserHistory& operator=(const BrowserHistory&) = delete;
        ~BrowserHistory(){
            delete_history()
        }
        string current_page(){
            if(current == nullptr) return "";
            Node* pointer=head;
            int page_num=1;
            while(pointer!=current){
                pointer=pointer->next;
                page_num++;
            }
            return current->url + " (page " + to_string(page_num) + " out of " + to_string(size) + ")";
        }
        void visit(string url){
            if(current == nullptr){
                head = new Node{nullptr, nullptr, url};
                current = head;
                size = 1;
                return;
            }
            Node* pointer = current->next;
            
            while (pointer != nullptr) {
                Node* next_page = pointer->next;
                delete pointer;
                size--;
                pointer = next_page;
            }
            
            Node* new_page= new Node{current,nullptr,url};
            current->next=new_page;
            current=new_page;
            size++;
            if(size>capacity){
                Node* deletion=head;
                head=head->next;
                delete deletion;
                head->prev=nullptr;
                size--;
            }
        }
        string back(int steps){
            if(current == nullptr) return "";
            for(int i=0;i<steps && current->prev!=nullptr;i++){
                current=current->prev;
            }
            return current->url;
        }
        string forward(int steps){
            if(current == nullptr) return "";
            for(int i=0;i<steps && current->next!=nullptr;i++){
                current=current->next;
            }
            return current->url;
        }
        void display_history(){
            Node* pointer=head;
            while(pointer!=nullptr){
                if(pointer==current)cout<< pointer->url<< "[<]"<< endl;
                else cout<<  pointer->url<<endl;
                pointer=pointer->next;
            }
        }
        void delete_history(){
            Node* pointer=head;
            while(pointer!=nullptr){
                Node* next_page = pointer->next;
                delete pointer;
                pointer = next_page;
            }
            size=0;
            head=current=nullptr;
        }
        bool delete_url(string url){
            if(head==nullptr)return false;
            if(head->url==url){
                s.push(url);
                if(current==head)current=current->next;
                Node* new1=head;
                head=head->next;
                if(head!=nullptr)head->prev=nullptr;
                delete new1;
                size--;
                return true;
            }
            Node* pointer=head->next;
            while(pointer!=nullptr){
                if(pointer->url==url){
                    s.push(url);
                    if(current == pointer){
                        if(pointer->next != nullptr)
                            current = pointer->next;
                    else
                        current = pointer->prev;
                    }
                    Node* new1=pointer;
                    pointer->prev->next=pointer->next;
                    if(pointer->next!=nullptr)pointer->next->prev=pointer->prev;
                    size--;
                    delete new1;
                    return true;
                }
                pointer=pointer->next;
            }
            return false;
        }
        bool restore(){
            if(s.empty())return false;
            string f=s.top();
            visit(f);
            s.pop();
            return true;
        }
        bool export_history(string filename){
            ofstream file(filename);
            if(!file) return false;

            Node* pointer = head;
            while(pointer != nullptr){
                file << pointer->url <<endl;
                pointer = pointer->next;
            }

            file.close();
            return bool(file);
                
        }
        bool import_history(string filename){
            ifstream file(filename);
            if(!file) return false;
            delete_history();
            string url;
            while(getline(file, url)){
                visit(url);
            }

        return file.eof() && !file.bad();
        }
};
int main(){
    BrowserHistory history("google.com");
    int choice;
    int steps;
    string url;
    string filename;

    while(true){
        cout<<"1. Visit a page"<<endl;
        cout<<"2. Back"<<endl;
        cout<<"3. Forward"<<endl;
        cout<<"4. Current page"<<endl;
        cout<<"5. Display history"<<endl;
        cout<<"6. Clear history"<<endl;
        cout<<"7. Delete a URL"<<endl;
        cout<<"8. Restore"<<endl;
        cout<<"9. Export"<<endl;
        cout<<"10. Import"<<endl;
        cout<<"0. Exit"<<endl;
        cout<<"Choose an option: ";
        
        if(!(cin >> choice)) break;
        
        if(choice == 0) break;

        switch(choice){
            case 1:
                cout<<"Enter URL: ";
                cin>>url;
                history.visit(url);
                break;

            case 2:
                cout<<"Enter steps: ";
                if(!(cin >> steps)) break;
                cout<<history.back(steps) <<endl;
                break;
            case 3:
                cout<<"Enter steps: ";
                if(!(cin >> steps)) break;
                cout<<history.forward(steps) <<endl;
                break;

            case 4:
                cout<<history.current_page() <<endl;
                break;

            case 5:
                history.display_history();
                break;

            case 6:
                history.delete_history();
                break;

            case 7:
                cout<<"Enter URL to delete: ";
                cin>>url;
                if(history.delete_url(url))
                    cout<<"Page deleted."<<endl;
                else
                    cout<<"URL not found."<<endl;
                break;

            case 8:
                if(history.restore())
                    cout<<"Page restored."<<endl;
                else
                    cout<<"Nothing to restore."<<endl;
                break;

            case 9:
                cout<<"Enter filename: ";
                cin>>filename;
                if(history.export_history(filename))
                    cout<<"History exported."<<endl;
                else
                    cout<<"Export failed."<<endl;
                break;

            case 10:
                cout<<"Enter filename: ";
                cin >>filename;
                if(history.import_history(filename))
                    cout<<"History imported."<<endl;
                else
                    cout<<"Import failed."<<endl;
                break;

            default:
                cout<<"Invalid option."<<endl;
        }
    }

    return 0;
}

