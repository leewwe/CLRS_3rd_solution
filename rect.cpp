#include <iostream>  
#include <algorithm>  
using namespace std;  
  
#define BLACK 0  
#define RED 1  
#define N 10//矩形的个数  
//区间结构  
struct interval  
{  
    int low;  
    int high;  
    interval(){};  
    interval(int l, int h):low(l), high(h){}  
    bool operator==(interval &b)  
    {  
        if(low == b.low && high == b.high)  
            return 1;  
        return 0;  
    }  
};  
//矩形结构  
struct Rectangle  
{  
    interval x;  
    interval y;  
};  
//用于排序  
struct Sort_Node  
{  
    int x;  
    int id;//x所属的矩形的编号  
    Sort_Node(){}  
    Sort_Node(int a, int b):x(a),id(b){}  
};  
//用于排序  
bool cmp(Sort_Node a, Sort_Node b)  
{  
    return a.x < b.x;  
}  
//区间树结点结构  
struct node  
{  
    node *left;  
    node *right;  
    node *p;  
    bool color;  
    interval inte;//仅存储矩形的y区间  
    int max;  
    node(node *init, interval i):left(init),right(init),p(init),inte(i),max(i.high),color(BLACK){}  
};  
//区间树结构  
struct Interval_Tree  
{  
    node *root;//根结点  
    node *nil;//哨兵  
    Interval_Tree(){nil = new node(NULL, interval(0,-0x7fffffff));root = nil;};  
};  
//a和b是否重叠，若重叠，返回1  
bool Overlap(interval a, interval b)  
{  
    //a在b的左边  
    if(a.high < b.low)  
        return 0;  
    //a在b的右边  
    if(a.low > b.high)  
        return 0;  
    return 1;  
}  
//用于维护信息  
int max(int a,int b,int c)  
{  
    if(a > b)  
        return a > c ?  a : c;  
    else  
        return b > c ? b : c;  
}  
//用于维护信息  
int Maintaining(node *z)  
{  
    if(z->inte.low > z->inte.high)  
        return 0;  
    return max(z->inte.high, z->left->max, z->right->max);  
}  
//搜索一个区间  
node *Interval_Search(Interval_Tree *T, interval i)  
{  
    //从根结点开始  
    node *x = T->root;  
    //不是叶子且不重叠  
    while(x != T->nil && !Overlap(i, x->inte))  
    {  
        //在左子树中  
        if(x->left != T->nil && x->left->max >= i.low)  
            x = x->left;  
        //在右子树中  
        else  
            x = x->right;  
    }  
    return x;  
}  
//左旋，令y = x->right, 左旋是以x和y之间的链为支轴进行旋转  
//涉及到的结点包括：x,y,y->left，令node={p,l,r},具体变化如下：  
//x={x->p,x->left,y}变为{y,x->left,y->left}  
//y={x,y->left,y->right}变为{x->p,x,y->right}  
//y->left={y,y->left->left,y->left->right}变为{x,y->left->left,y->left->right}  
void Left_Rotate(Interval_Tree *T, node *x)  
{  
    //令y = x->right  
    node *y = x->right;  
    //按照上面的方式修改三个结点的指针，注意修改指针的顺序  
    x->right = y->left;  
    if(y->left != T->nil)  
        y->left->p = x;  
    y->p = x->p;  
    if(x->p == T->nil)//特殊情况：x是根结点  
        T->root = y;  
    else if(x == x->p->left)  
        x->p->left = y;  
    else   
        x->p->right = y;  
    y->left = x;  
    x->p = y;  
    //维护信息  
    Maintaining(x);  
    Maintaining(y);  
}  
//右旋，令y = x->left, 左旋是以x和y之间的链为支轴进行旋转  
//旋转过程与上文类似  
void Right_Rotate(Interval_Tree *T, node *x)  
{  
    node *y = x->left;  
    x->left = y->right;  
    if(y->right != T->nil)  
        y->right->p = x;  
    y->p = x->p;  
    if(x->p == T->nil)  
        T->root = y;  
    else if(x == x->p->right)  
        x->p->right = y;  
    else   
        x->p->left = y;  
    y->right = x;  
    x->p = y;  
    //维护信息  
    Maintaining(x);  
    Maintaining(y);  
}  
//红黑树调整  
void Interval_Tree_Insert_Fixup(Interval_Tree *T, node *z)  
{  
    node *y;  
    //唯一需要调整的情况，就是违反性质2的时候，如果不违反性质2，调整结束  
    while(z->p->color == RED)  
    {  
        //p[z]是左孩子时，有三种情况  
        if(z->p == z->p->p->left)  
        {  
            //令y是z的叔结点  
            y = z->p->p->right;  
            //第一种情况，z的叔叔y是红色的  
            if(y->color == RED)  
            {  
                //将p[z]和y都着为黑色以解决z和p[z]都是红色的问题  
                z->p->color = BLACK;  
                y->color = BLACK;  
                //将p[p[z]]着为红色以保持性质5  
                z->p->p->color = RED;  
                //把p[p[z]]当作新增的结点z来重复while循环  
                z = z->p->p;  
            }  
            else  
            {  
                //第二种情况：z的叔叔是黑色的，且z是右孩子  
                if(z == z->p->right)  
                {  
                    //对p[z]左旋，转为第三种情况  
                    z = z->p;  
                    Left_Rotate(T, z);  
                }  
                //第三种情况：z的叔叔是黑色的，且z是左孩子  
                //交换p[z]和p[p[z]]的颜色，并右旋  
                z->p->color = BLACK;  
                z->p->p->color = RED;  
                Right_Rotate(T, z->p->p);  
            }  
        }  
        //p[z]是右孩子时，有三种情况，与上面类似  
        else if(z->p == z->p->p->right)  
        {  
            y = z->p->p->left;  
            if(y->color == RED)  
            {  
                z->p->color = BLACK;  
                y->color = BLACK;  
                z->p->p->color = RED;  
                z = z->p->p;  
            }  
            else  
            {  
                if(z == z->p->left)  
                {  
                    z = z->p;  
                    Right_Rotate(T, z);  
                }  
                z->p->color = BLACK;  
                z->p->p->color = RED;  
                Left_Rotate(T, z->p->p);  
            }  
        }  
    }  
    //根结点置为黑色  
    T->root->color = BLACK;  
}  
//插入一个结点  
void Interval_Tree_Insert(Interval_Tree *T, node *z)  
{  
    node *y = T->nil, *x = T->root;  
    //找到应该插入的位置，与二叉查找树的插入相同  
    while(x != T->nil)  
    {  
        y = x;  
        if(z->inte.low < x->inte.low)  
            x = x->left;  
        else  
            x = x->right;  
    }  
    z->p = y;  
    if(y == T->nil)  
        T->root = z;  
    else if(z->inte.low < y->inte.low)  
        y->left = z;  
    else  
        y->right = z;  
    z->left = T->nil;  
    z->right = T->nil;  
    //将新插入的结点转为红色  
    z->color = RED;  
    //从新插入的结点开始，向上调整  
    Interval_Tree_Insert_Fixup(T, z);  
}  
//对树进行调整，x指向一个红黑结点，调整的过程是将额外的黑色沿树上移  
void Interval_Tree_Delete_Fixup(Interval_Tree *T, node *x)  
{  
    node *w;  
    //如果这个额外的黑色在一个根结点或一个红结点上，结点会吸收额外的黑色，成为一个黑色的结点  
    while(x != T->root && x->color == BLACK)  
    {  
        //若x是其父的左结点（右结点的情况相对应）  
        if(x == x->p->left)  
        {  
            //令w为x的兄弟，根据w的不同，分为三种情况来处理  
            //执行删除操作前x肯定是没有兄弟的，执行删除操作后x肯定是有兄弟的  
            w = x->p->right;  
            //第一种情况：w是红色的  
            if(w->color == RED)  
            {  
                //改变w和p[x]的颜色  
                w->color = BLACK;  
                x->p->color = RED;  
                //对p[x]进行一次左旋  
                Left_Rotate(T, x->p);  
                //令w为x的新兄弟  
                w = x->p->right;  
                //转为2.3.4三种情况之一  
            }  
            //第二情况：w为黑色，w的两个孩子也都是黑色  
            if(w->left->color == BLACK && w->right->color == BLACK)  
            {  
                //去掉w和x的黑色  
                //w只有一层黑色，去掉变为红色，x有多余的一层黑色，去掉后恢复原来颜色  
                w->color = RED;  
                //在p[x]上补一层黑色  
                x = x->p;  
                //现在新x上有个额外的黑色，转入for循环继续处理  
            }  
            //第三种情况，w是黑色的,w->left是红色的,w->right是黑色的  
            else  
            {  
                if(w->right->color == BLACK)  
                {  
                    //改变w和left[x]的颜色  
                    w->left->color = BLACK;  
                    w->color = RED;  
                    //对w进行一次右旋  
                    Right_Rotate(T, w);  
                    //令w为x的新兄弟  
                    w = x->p->right;  
                    //此时转变为第四种情况  
                }  
                //第四种情况：w是黑色的,w->left是黑色的,w->right是红色的  
                //修改w和p[x]的颜色  
                w->color =x->p->color;  
                x->p->color = BLACK;  
                w->right->color = BLACK;  
                //对p[x]进行一次左旋  
                Left_Rotate(T, x->p);  
                //此时调整已经结束，将x置为根结点是为了结束循环  
                x = T->root;  
            }  
        }  
        //若x是其父的左结点（右结点的情况相对应）  
        else if(x == x->p->right)  
        {  
            //令w为x的兄弟，根据w的不同，分为三种情况来处理  
            //执行删除操作前x肯定是没有兄弟的，执行删除操作后x肯定是有兄弟的  
            w = x->p->left;  
            //第一种情况：w是红色的  
            if(w->color == RED)  
            {  
                //改变w和p[x]的颜色  
                w->color = BLACK;  
                x->p->color = RED;  
                //对p[x]进行一次左旋  
                Right_Rotate(T, x->p);  
                //令w为x的新兄弟  
                w = x->p->left;  
                //转为2.3.4三种情况之一  
            }  
            //第二情况：w为黑色，w的两个孩子也都是黑色  
            if(w->right->color == BLACK && w->left->color == BLACK)  
            {  
                //去掉w和x的黑色  
                //w只有一层黑色，去掉变为红色，x有多余的一层黑色，去掉后恢复原来颜色  
                w->color = RED;  
                //在p[x]上补一层黑色  
                x = x->p;  
                //现在新x上有个额外的黑色，转入for循环继续处理  
            }  
            //第三种情况，w是黑色的,w->right是红色的,w->left是黑色的  
            else  
            {  
                if(w->left->color == BLACK)  
                {  
                    //改变w和right[x]的颜色  
                    w->right->color = BLACK;  
                    w->color = RED;  
                    //对w进行一次右旋  
                    Left_Rotate(T, w);  
                    //令w为x的新兄弟  
                    w = x->p->left;  
                    //此时转变为第四种情况  
                }  
                //第四种情况：w是黑色的,w->right是黑色的,w->left是红色的  
                //修改w和p[x]的颜色  
                w->color =x->p->color;  
                x->p->color = BLACK;  
                w->left->color = BLACK;  
                //对p[x]进行一次左旋  
                Right_Rotate(T, x->p);  
                //此时调整已经结束，将x置为根结点是为了结束循环  
                x = T->root;  
            }  
        }  
    }  
    //吸收了额外的黑色  
    x->color = BLACK;  
}  
//找最小值     
node *Tree_Minimum(Interval_Tree *T, node *x)    
{    
    //只要有比当前结点小的结点     
    while(x->left != T->nil)    
        x = x->left;    
    return x;    
}   
//查找中序遍历下x结点的后继，后继是大于key[x]的最小的结点     
node *Tree_Successor(Interval_Tree *T, node *x)    
{    
    //如果有右孩子     
    if(x->right != T->nil)    
        //右子树中的最小值     
        return Tree_Minimum(T, x->right);    
    //如果x的右子树为空且x有后继y，那么y是x的最低祖先结点，且y的左儿子也是     
    node *y = x->p;    
    while(y != NULL && x == y->right)    
    {    
        x = y;    
        y = y->p;    
    }    
    return y;    
}    
//递归地查询二叉查找树     
node *Interval_Tree_Search(node *x, interval k)    
{    
    //找到叶子结点了还没找到，或当前结点是所查找的结点     
    if(x->inte.high < x->inte.low || k == x->inte)    
        return x;    
    //所查找的结点位于当前结点的左子树     
    if(k.low < x->inte.low)    
        return Interval_Tree_Search(x->left, k);    
    //所查找的结点位于当前结点的左子树     
    else    
        return Interval_Tree_Search(x->right, k);    
}   
//红黑树的删除  
node *Interval_Tree_Delete(Interval_Tree *T, node *z)  
{  
    //找到结点的位置并删除，这一部分与二叉查找树的删除相同  
    node *x, *y;  
    if(z->left == T->nil || z->right == T->nil)  
        y = z;  
    else y = Tree_Successor(T, z);  
    if(y->left != T->nil)  
        x = y->left;  
    else x = y->right;  
    x->p = y->p;  
    if(y->p == T->nil)  
        T->root = x;  
    else if(y == y->p->left)  
        y->p->left = x;  
    else  
        y->p->right = x;  
    Maintaining(y->p);  
    if(y != z)  
    {  
        z->inte = y->inte;  
        Maintaining(z);  
    }  
    //如果被删除的结点是黑色的，则需要调整  
    if(y->color == BLACK)  
        Interval_Tree_Delete_Fixup(T, x);  
    return y;  
}  
//求与z区间相交的区间的个数  
int Interval_Tree_Sum(node *r, interval z)  
{  
    //叶子结点  
    if(r->inte.low > r->inte.high)  
        return 0;  
    //不相交  
    if(z.high < r->inte.low)  
        return Interval_Tree_Sum(r->left, z);  
    if(z.low > r->inte.high)  
        return Interval_Tree_Sum(r->right, z);  
    //相交  
    int ret = 1;  
    //不相交的部分继续求相交个数  
    if(z.low < r->inte.low)  
    {  
        interval t(z.low, r->inte.low);  
        ret = ret + Interval_Tree_Sum(r->left, t);  
    }  
    if(z.high > r->inte.high)  
    {  
        interval t(r->inte.high, z.high);  
        ret = ret + Interval_Tree_Sum(r->right, t);  
    }  
    return ret;  
}  
/* 
1 3 2 4 
2 4 1 5 
*/  
int main()  
{  
    //生成一棵区间树  
    Interval_Tree *T = new Interval_Tree;  
    //记录所有的矩形  
    Rectangle rect[N];  
    //用于排序  
    Sort_Node Sn[2*N];  
    int i;  
    for(i = 0; i < N; i++)  
    {  
        //输入矩形  
        cin>>rect[i].x.low>>rect[i].x.high>>rect[i].y.low>>rect[i].y.high;  
        //把x收集起来用于排序  
        Sn[2*i] = Sort_Node(rect[i].x.low, i);  
        Sn[2*i+1] = Sort_Node(rect[i].x.high, i);  
    }  
    //按照x值排序  
    sort(Sn, Sn + 2*N, cmp);  a
    int ans = 0;  
    //依次处理排序结果  
    for(i = 0; i < 2 * N; i++)  
    {  
        int x = Sn[i].x;  
        int id = Sn[i].id;  
        //如果x是矩形i的左边x  
        if(x == rect[id].x.low)  
        {  
            //求出区间树中与矩形i的y区间相交的区间个数  
            ans = ans + Interval_Tree_Sum(T->root, rect[id].y);  
            //把矩形i的y区间插入到区间树中  
            node *z = new node(T->nil, rect[id].y);  
            Interval_Tree_Insert(T, z);  
        }  
        //如果x是矩形i的右边x  
        else if(x == rect[id].x.high)  
        {  
            //将矩形i的y区间从区间树中删除  
            node *ret = Interval_Tree_Search(T->root, rect[id].y);  
            if(ret != T->nil)  
                Interval_Tree_Delete(T, ret);  
        }  
    }  
    //输出结果  
    cout<<ans<<endl;  
    return 0;  
} 
