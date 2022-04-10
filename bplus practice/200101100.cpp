#include <bits/stdc++.h>

using namespace std;

struct node
{
    bool is_leaf;
    int size;
    int* index_values;
    int* data_values;
    node** pointers;
    node* p;
};

class b_tree
{
    int d;
    int t;
    node* root;
    int number_index;
    int number_data;

    public:
    
    void init(int d_in, int t_in)
    {   
        d=d_in;
        t=t_in;
        root = new node;
        root->is_leaf = true;
        root->data_values = (int*)malloc(2*d_in*sizeof(int));
        root->size = 0;
        root->p = NULL;
        root->pointers = NULL;
        number_data = 1;
        number_index = 0;
    }

    bool is_full(node* noder)
    {
        if(noder->is_leaf)
        {
            return (noder->size == (2*d));
        }
        else
        {
            return (noder->size == (2*t+1));
        }
    }

    node* return_leaf()
    {
        node* neww = new node;
        neww->is_leaf = true;
        neww->data_values = (int*)malloc(2*d*sizeof(int));
        neww->index_values = NULL;
        neww->pointers = NULL;
        neww->size = 0;
        neww->p = NULL;

        return neww;
    }

    node* return_index()
    {   
        node* neww = new node;
        neww->is_leaf = false;
        neww->index_values = (int*)malloc((2*t+1)*sizeof(int));
        neww->data_values = NULL;
        neww->pointers = (node**)malloc((2*t+2)*sizeof(node*));
        neww->size = 0;
        neww->p = NULL;

        
        return neww;
        
    }

    void insert(node* noder,int key)
    {
        noder->data_values[noder->size] = key;
        (noder->size)++;
    }

    void insert_real(int key)
    {   
        
        node* noder = search(key);

        if(!(is_full(noder))) // node is not full
        {   
            noder->data_values[noder->size] = key;
            (noder->size)++;
        }
        else // node is full
        {   
            int temp_ar[2*d+1];
            temp_ar[2*d] = key;
            node* noder2 = return_leaf();
            number_data++;

            for(int i=0; i<(2*d); i++)
            {
                temp_ar[i] = noder->data_values[i];
            }

            sort(temp_ar,temp_ar+(2*d+1));

            noder->size = d;
            noder2->size = d+1;

            for(int i=0; i<d; i++)
            {
                noder->data_values[i] = temp_ar[i];
            }

            for(int i=d; i<=(2*d); i++)
            {
                noder2->data_values[i-d] = temp_ar[i];
            }

            // By now, tree structure is unaltered and a new node with second half of the contents has been created

            if(noder==root) // if split node was the root, we create a new index root
            {
                node* new_root = return_index();
                number_index++;
                noder->p = new_root;
                noder2->p = new_root;
                root = new_root;
                root->index_values[0] = noder2->data_values[0];
                (root->size)++;
                root->pointers[0] = noder;
                root->pointers[1] = noder2;
            }
            else // split node was not the root
            {   
                insert_into_index(noder2,noder->p,noder2->data_values[0]);
            }
        }
        // cout<<"=============";
        // print_node(root->pointers[0]);
        // cout<<"-------------";
        // print_node(root->pointers[1]);
        // cout<<"=============";
    }

    void insert_extend(node* noder, node* parent)
    {
        int key = noder->data_values[0];
        int i=0;

        while((i<(parent->size)) && (key >= (parent->index_values[i])))
        {
            i++;
        }

        for(int j=((parent->size)-1); j>=i; j--)
        {
            parent->index_values[j+1] = parent->index_values[j];
        }
        for(int k=((parent->size)); k>=(i+1); k--)
        {
            parent->pointers[k+1] = parent->pointers[k];
        }
        parent->index_values[i] = key;
        parent->pointers[i+1] = noder;
        noder->p = parent;
        (parent->size)++;
        
    }   
    void insert_into_index(node* noder,node* parent,int key)
    {
        int i=0;
        while((i<(parent->size)) && (key >= (parent->index_values[i])))
        {
            i++;
        }

        if(!is_full(parent))
        {
            for(int j=((parent->size)-1); j>=i; j--)
            {
                parent->index_values[j+1] = parent->index_values[j];
            }
            for(int k=((parent->size)); k>=(i+1); k--)
            {
                parent->pointers[k+1] = parent->pointers[k];
            }
            parent->index_values[i] = key;
            parent->pointers[i+1] = noder;
            noder->p = parent;
            (parent->size)++;
        }
        else
        {
            node* extended_index = new node;
            extended_index->is_leaf = false;
            extended_index->index_values = (int*)malloc((2*t+2)*sizeof(int));
            extended_index->data_values = NULL;
            extended_index->pointers = (node**)malloc((2*t+3)*sizeof(node*));
            extended_index->size = 0;
            extended_index->p = NULL;

            for(int i=0; i<(parent->size); i++)
            {
                extended_index->index_values[i] = parent->index_values[i];
                extended_index->pointers[i] = parent->pointers[i];
            }
            extended_index->pointers[parent->size] = parent->pointers[parent->size];

            extended_index->size = (2*t+1);
            insert_extend(noder,extended_index);

            node* parent2 = return_index();
            number_index++;
            
            parent2->size = t+1;
            parent->size = t;

            for(int i=0; i<(parent->size); i++)
            {
                parent->index_values[i] = extended_index->index_values[i];
                parent->pointers[i] = extended_index->pointers[i];
            }
            
            parent->pointers[parent->size] = extended_index->pointers[parent->size];

            for(int i=0; i<(parent2->size); i++)
            {
                parent2->index_values[i] = extended_index->index_values[i+t+1];
                parent2->pointers[i] = extended_index->pointers[i+t+1];
            }
            parent2->pointers[parent2->size] = extended_index->pointers[(parent2->size)+t+1];

            make_children(parent);
            make_children(parent2);


            if(parent == root || parent->p == NULL)
            {
                node* new_root = return_index();
                number_index++;
                parent->p = new_root;
                parent2->p = new_root;
                root = new_root;
                root->index_values[0] = extended_index->index_values[t];
                (root->size)++;
                root->pointers[0] = parent;
                root->pointers[1] = parent2;
            }
            else
            {
                insert_into_index(parent2,parent->p,extended_index->index_values[t]);
            }
        };
    }

    void make_children(node* noder)
    {
        for(int i=0; i<=(noder->size); i++)
        {
            noder->pointers[i]->p = noder;
        }
    }

    void print_node(node* noder)
    {   
        if(noder->is_leaf)
        {
            for(int i=0; i<(noder->size); i++)
            {  
                cout<<noder->data_values[i]<<" ";
            }
            //cout<<"|| "<<noder->size<<"\n";
            cout<<"\n";
        }
        else
        {
            for(int i=0; i<(noder->size); i++)
            {  
                cout<<noder->index_values[i]<<" ";
            }
            //cout<<"|| "<<noder->size<<"\n";
            cout<<"\n";
        }
        
    }

    void print_tree(node* nodeptr)
    {   
        print_node(nodeptr);
        if(!nodeptr->is_leaf)
        {
            for(int i=0; i<=(nodeptr->size); i++)
            {
                print_tree(nodeptr->pointers[i]);
            }
        }
        
    }

    void status()
    {
        cout<<number_index<<" "<<number_data<<" ";
        print_node(root);
    }

    void testing()
    {
        print_tree(root);
        cout<<"------"<<number_data<<" "<<number_index<<"-------"<<"\n";
    }

    node* search(int key)
    {   
        return search_subtree(root,key);
    }

    node* search_subtree(node* nodeptr,int key)
    {   
        if(nodeptr->is_leaf)
            return nodeptr;
        else
        {
            int i=0;
            while((i<(nodeptr->size)) && (key >= (nodeptr->index_values[i])))
            {
                i++;
            }
            return search_subtree(nodeptr->pointers[i],key);
        }

        return NULL;
    }
};

int main()
{
    b_tree epic;
    
    int d,t;
    cin>>d;
    cin>>t;

    epic.init(d,t);

    int command, operand;
    while(1)
    {
        cin>>command;

        if((command!=2)&&(command!=3))
            cin>>operand;

        switch (command)
        {   
            case 1: {epic.insert_real(operand); break;}
            case 2: {epic.status(); break;}
            case 3: return 0;
            default: break;
        }

    }

    // epic.insert_real(1);
    // epic.insert_real(2);
    // epic.insert_real(3);
    // epic.insert_real(4);
    // epic.insert_real(5);
    // epic.insert_real(6);
    // epic.insert_real(7);
    // epic.insert_real(8);
    // epic.insert_real(9);
    // epic.insert_real(10);
    // epic.insert_real(11);
    // epic.insert_real(12);
    // epic.insert_real(13);
    // epic.insert_real(14);
    // epic.insert_real(15);
    // epic.testing();
}