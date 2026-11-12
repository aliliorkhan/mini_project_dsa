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

struct DeletedPage {
    Node* node;
    int position;
};

class BrowserHistory{
    private:
        stack<DeletedPage> s;
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
            delete_history();
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
            Node* pointer = head;

            while(pointer != nullptr){
                Node* next_page = pointer->next;
                delete pointer;
                pointer = next_page;
            }

            while(!s.empty()){
                delete s.top().node;
                s.pop();
            }

            size = 0;
            head = current = nullptr;
        }
        bool delete_url(string url){
            Node* pointer = head;
            int position = 0;

             while(pointer != nullptr){
                if(pointer->url == url){

                    if(current == pointer){
                        if(pointer->next != nullptr)
                            current = pointer->next;
                        else
                            current = pointer->prev;
                    }

                    if(pointer->prev != nullptr)
                        pointer->prev->next = pointer->next;
                    else
                        head = pointer->next;

                    if(pointer->next != nullptr)
                        pointer->next->prev = pointer->prev;

                    pointer->prev = nullptr;
                    pointer->next = nullptr;
                    s.push({pointer, position});

                    size--;
                    return true;
                }

                pointer = pointer->next;
                position++;
            }       

            return false;
        }

        bool restore(){
            if(s.empty() || size >= capacity)
                return false;

            DeletedPage deleted = s.top();
            s.pop();

            Node* node = deleted.node;
            int position = deleted.position;

            if(head == nullptr){
                node->prev = nullptr;
                node->next = nullptr;
                head = node;
                current = node;
            }

            else if(position == 0){
                node->prev = nullptr;
                node->next = head;
                head->prev = node;
                head = node;
            }

            else{
                Node* before = head;

                for(int i = 1; i < position && before->next != nullptr; i++){
                    before = before->next;
                }

                node->prev = before;
                node->next = before->next;

                if(node->next != nullptr)
                    node->next->prev = node;

                before->next = node;
            }

            size++;
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

