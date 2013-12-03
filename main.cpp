#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <boost/tuple/tuple.hpp>
#include <boost/shared_ptr.hpp>
#include <list>
#include <fstream>

using namespace std;
using namespace boost;

struct Product{
    int size;
    int id;
};


void read(istream& in, int& n, int& k,  int& c, vector< list<int> >& boxes, vector<Product>& products  ){
    in >> n >> c;
    for(int i = 0; i < n; ++i){
        list<int> t;
        t.push_back(0);
        int b;
        in >> b;

        for (int j = 0; j < b; ++j){
            int r;
            in >> r;
            t.push_back(r);
        }

        t.push_back(c);
        boxes.push_back(t);
    }

    in >> k;

    for(int i = 0; i < k; ++i){
        int t1,t2;
        Product p;
        p.id = i;
        in >> t1 >> t2 >> p.size;
        products.push_back(p);
    }
}

void readMore(istream& in, int& n, int& k,  int& c, vector< list<int> >& boxes, vector<Product>& products,vector<int>& fullness, vector<vector<int> >& settlment  ){
    in >> n >> c;
    for(int i = 0; i < n; ++i){
        list<int> t;
        t.push_back(0);
        int b;
        in >> b;

        for (int j = 0; j < b; ++j){
            int r;
            in >> r;
            t.push_back(r);
        }

        t.push_back(c);
        boxes.push_back(t);
    }

    fullness.clear();
    settlment.clear();
    fullness.resize(n);
    settlment.resize(n);
    in >> k;

    for(int i = 0; i < k; ++i){
        int t1,t2;
        Product p;
        p.id = i;
        in >> t1 >> t2 >> p.size;
        products.push_back(p);
        fullness[t1] += p.size;
        settlment[t1].push_back(i);
    }
}


bool prodComp(const Product& a, const Product& b){
    return a.size > b.size;
}
template<typename Iter>
int mult(Iter begin, Iter end){
    int res = 1;
    while(begin != end)
        res *= *(begin++);
    return res;
}

void greedBalc(vector< list<int> >& boxes, vector<Product>& products, int c ){
    int n = products.size();
    boxes.clear();
    sort(products.begin(),products.end(),prodComp);
    int count = 0;
    vector<int> marks(n);
    fill_n(marks.begin(),n,0);
    // Possibly an infinity cicle here
    while(count != n){
        int ls = 0;
        list<int> b;
        boxes.push_back(b);
 //       boxes.rbegin()->push_back(ls);
        for(int i = 0; i < n; ++i){
            if (!marks[i] && ls + products[i].size <= c){
                boxes.rbegin()->push_back(ls);
                ls += products[i].size;
                marks[i] = 1;
                ++count;
            }
        }
        boxes.rbegin()->push_back(c);
    }
}

void fillBox(list<int>& box, vector<Product>& products, int& max, int c ){
    c++;
    int** a = new int*[c];
    for(int i = 0; i < c; ++i)
        a[i] = new int[max];

    for (int i = 0; i < max; ++i)
        a[0][i] = 0;
    for(int i = 0; i < products[0].size; ++i){
        a[i][0] = 0;
    }
    for(int i = products[0].size; i < c ; ++i){
        a[i][0] = products[0].size;
    }

    for (int j = 1; j < max; ++j){
        for (int i = 1; i < c; ++i){
            if (products[j].size > i){
                a[i][j] = a[i][j-1];
            }else{
                a[i][j] =  (a[i][j-1] >= a[ i-products[j].size ][j-1] + products[j].size ? a[i][j-1] : a[ i-products[j].size ][j-1] + products[j].size);
            }
        }
    }
 /*   for (int i = 0; i < c; ++i){
        for (int j = 0; j < max; ++j){
            cout << a[i][j] << ' ';
        }
        cout << endl;
    }
    for (int i = 0; i < max; ++i )
        cout << products[i].size << endl;
*/   box.clear();
    box.push_back(0);
    int d = c-1, b = max-1;
    while(d != 0 || b != 0){
        if (b > 0){
            if (products[b].size <= d){
//                cout << b << ' ' << d << ' ' << products[b].size << endl;
                if (products[b].size == a[d][b] - a[d-products[b].size][b-1]){
                    box.push_back(*(box.rbegin()) + products[b].size);
                    swap(products[b],products[--max]);
                    d -= products[b].size;
                    --b;
                }else{
                    --b;
                }
            }else{
                --b;
            }
        }else{
            if (products[b].size <= d){
                if (products[b].size == a[d][b] - a[d-products[b].size][b-1]){
                    box.push_back(*(box.rbegin()) + products[b].size);
                    swap(products[b],products[--max]);
                    d -= products[b].size;
                }
            }
            d = 0;
        }
    }

    for(int i = 0; i < c; ++i)
        delete[] a[i];
    delete[] a;
}

void optBalc(vector< list<int> >& boxes, vector<Product> products, int c ){
    int n = products.size();
    boxes.clear();
    int max = n;
    // Possibly an infinity cicle here
    while(max){
//      cout << max << endl;
        list<int> b;
        fillBox(b,products,max,c);
//      copy(b.begin(),b.end(),ostream_iterator<int>(cout, ","));
        boxes.push_back(b);
    }
}

inline int findNotmarkedMax(const vector<int>& v, const vector<int>& marks,int c){
    vector<int>::const_iterator itv = v.begin();
    vector<int>::const_iterator itm = marks.begin();
    vector<int>::const_iterator endv = v.end();
    int max = 0,argmax = c+1,i = 0;
    for (; itv != endv; ++itv,++itm,++i){
        if ((*itv < argmax) && !(*itm)){
            max = i;
            argmax = *itv;
        }
    }
    return max;
}

vector<int> resetBalc(int k,vector< list<int> >& boxes, vector<Product>& products, int c,vector<int>& fullness, vector<vector<int> >& settlment ){
    vector<int> repearedBoxes;
    vector<int> marks(fullness.size());
    fill_n(marks.begin(),fullness.size(),0);
    vector<Product> newProducts;
    for (int i = 0; i < k; ++i){
        int max = findNotmarkedMax(fullness,marks,c);
 //       cout << "max: " << max << endl;
        marks[max] = 1;
        repearedBoxes.push_back(max);
        vector<int>::iterator it = settlment[max].begin();
        vector<int>::iterator end = settlment[max].end();
        for (; it != end; ++it){
            newProducts.push_back(products[*it]);
        }
    }
/*    cout << "New products" << endl;
    for (int i = 0; i < newProducts.size(); ++i){
        cout << newProducts[i].id << ' ' << newProducts[i].size << endl;
    }
    cout << endl;
*/
    optBalc(boxes,newProducts,c);

    return repearedBoxes;
}



int main()
{

    ifstream in("storage1.txt");

    int n,k,c;
    vector< list<int> > boxes;
    vector<Product> products;
    vector<int> fullness;
    vector<vector<int> > settlment;
    readMore(in,n,k,c,boxes,products,fullness,settlment);
 /*   for (int i = 0; i < n; ++i){
        copy(settlment[i].begin(),settlment[i].end(),ostream_iterator<int>(cout, ","));
        cout << endl;
    }*/
    vector< list<int> > oboxes;
    optBalc(oboxes,products,c);
    cout << "*************************************************" << endl << """**************optimal solution*******************" << endl << "*************************************************" << endl;
    for (int i = 0; i < oboxes.size(); ++i){
        cout << '[' ;
        copy(oboxes[i].begin(),oboxes[i].end(),ostream_iterator<int>(cout, ","));
        cout << "]" << endl;
    }

    for (int i = 1; i <= n; ++i){
        cout << "*************************************************" << endl << """**************reseted boxes**********************" << endl << "*************************************************" << endl;
        vector<int> resetedBoxes = resetBalc(i,oboxes,products,c,fullness,settlment);
        copy(resetedBoxes.begin(),resetedBoxes.end(),ostream_iterator<int>(cout, ","));
        cout << endl;
        for (int i = 0; i < oboxes.size(); ++i){
            cout << '[' ;
            copy(oboxes[i].begin(),oboxes[i].end(),ostream_iterator<int>(cout, ","));
            cout << "]" << endl;
        }
        cout << "Improvement: " << resetedBoxes.size() - oboxes.size() << endl;
    }


    return 0;
}

