#pragma once

#include "_common.h"

#include <iostream>
#include <queue>
#include <map>
#include <string>


inline void _register_misc(VM* vm, PyObject* mod){
    // Todo
}


//namespace huffman {

using uchar = unsigned char;

struct Node {
    uchar c;
    int freq;
    Node* left;
    Node* right;
    Node(uchar _c, int f, Node* l = nullptr, Node* r = nullptr)
        : c(_c), freq(f), left(l), right(r) {}
    bool operator<(const Node& node) const { //重载，优先队列的底层数据结构std::heap是最大堆
        return freq > node.freq;
    }
};

class huffTree {
public:
    huffTree(const std::map<uchar, int>& afMap) {
        for (auto i : afMap) {
            Node n(i.first, i.second);
            q.push(n);
        }
        makehuffTree();
    }
    ~huffTree() {
        Node node = q.top();
        _deleteTree(node.left);
        _deleteTree(node.right);
    }
    void makehuffTree() {
        while (q.size() != 1) {
            Node* left = new Node(q.top()); q.pop();
            Node* right = new Node(q.top()); q.pop();
            Node node('R', left->freq + right->freq, left, right);
            q.push(node);
        }
    }
    void huffmanCode(std::map<uchar, std::string>& codeMap) {
        Node node = q.top();
        std::string prefix;
        _huffmanCode(&node, prefix, codeMap);
    }
    Node getHuffTree() {
        return q.top();
    }
    void watch() {
        /*Node node = q.top();
        _treeWatch(&node);*/
    }
private:
    struct nodeBacklog {  //回溯点
        Node* node;
        int next_sub_idx;
    };
    enum {
        LeftIndex,
        RightIndex
    };
    enum { MaxLevel = 64 };

    /*
    * 以下用于实现树形打印二叉树，手动实现栈功能，原因是std::stack无法随机访问栈空间，
    * 而程序需要用到随机访问占空间的功能，这里用局部定义栈数组，能自动释放空间，
    * 这里的实现对深层次理解指针和地址的关系非常有帮助，个人觉得非常巧妙，值得学习
    */
    static void nbl_push(nodeBacklog* nbl, nodeBacklog** top, nodeBacklog** bottom) {
        if (*top - *bottom < MaxLevel) {
            (*(*top)++) = *nbl;  //这里其实已经将nbl的值拷贝到*top指向的位置，不能直接*top = nbl，否则会随nbl变化
        }
    }
    static nodeBacklog* nbl_pop(nodeBacklog** top, nodeBacklog** bottom) {
        return *top > *bottom ? -- * top : nullptr;
    }
    static bool _isLeaf(Node* n) {
        return n->left == nullptr && n->right == nullptr;
    }

private:
    void _deleteTree(Node* n) {
        if (!n) return;
        _deleteTree(n->left);
        _deleteTree(n->right);
        delete n;
    }
    void _treeWatch(Node* root);
    static void _printNode(Node* n) {
        if (!n) {
            printf("%c:%d\n", n->c, n->freq);
        }
    }
    void _huffmanCode(Node* root, std::string& prefix,
        std::map<uchar, std::string>& codeMap) {
        std::string tmp = prefix;
        if (root->left != nullptr) {
            prefix += '0';
            if (_isLeaf(root->left)) {
                codeMap[root->left->c] = prefix;
            }
            else {
                _huffmanCode(root->left, prefix, codeMap);
            }
        }
        if (root->right != nullptr) {
            prefix = tmp;
            prefix += '1';
            if (_isLeaf(root->right)) {
                codeMap[root->right->c] = prefix;
            }
            else {
                _huffmanCode(root->right, prefix, codeMap);
            }
        }
    }
private:
    std::priority_queue<Node> q;
};

void huffTree::_treeWatch(Node* root) {
    int level = 0;
    Node* node = root;
    nodeBacklog nbl;
    nodeBacklog* p_nbl = nullptr;
    nodeBacklog* top, * bottom, nbl_stack[MaxLevel];
    top = bottom = nbl_stack;

    for (;;) {
        if (node != nullptr) {
            int sub_index = p_nbl != nullptr ? p_nbl->next_sub_idx : RightIndex;
            p_nbl = nullptr;
            if (_isLeaf(node) || sub_index == LeftIndex) {
                nbl.node = nullptr;
                nbl.next_sub_idx = RightIndex;
            }
            else {
                nbl.node = node;
                nbl.next_sub_idx = LeftIndex;
            }
            nbl_push(&nbl, &top, &bottom);
            level++;
            if (sub_index == RightIndex) {
                for (int i = 1; i < level; ++i) {
                    if (i == level - 1) {
                        printf("%-8s", "|-------");
                    }
                    else {
                        if (nbl_stack[i - 1].node != nullptr) {
                            printf("%-8s", "|");
                        }
                        else {
                            printf("%-8s", " ");
                        }
                    }
                }
                _printNode(node);
            }
            node = sub_index == LeftIndex ? node->left : node->right;
        }
        else {
            p_nbl = nbl_pop(&top, &bottom);
            if (p_nbl == nullptr)
                break;
            node = p_nbl->node;
            level--;
        }
    }
}

//得到index位的值，若index位为0，则GET_BYTE值为假，否则为真
#define GET_BYTE(vbyte, index) (((vbyte) & (1 << ((index) ^ 7))) != 0)
//index位置1
#define SET_BYTE(vbyte, index) ((vbyte) |= (1 << ((index) ^ 7)))
//index位置0
#define CLR_BYTE(vbyte, index) ((vbyte) &= (~(1 << ((index) ^ 7))))

struct fileHead {
	char flag[4];				//压缩二进制文件头部标志 ycy
	uchar alphaVariety;		    //字符种类
	uchar lastValidBit;		    //最后一个字节的有效位数
	char unused[10];			//待用空间
};								//这个结构体总共占用16个字节的空间

struct alphaFreq {
	uchar alpha;		        //字符,考虑到文件中有汉字，所以定义成uchar
	int freq;	                //字符出现的频度
	alphaFreq() {}
	alphaFreq(const std::pair<char, int>& x)
		: alpha(x.first), freq(x.second) {}
};

class huffEncode {
public:
    bool encode(const char* srcFilename, const char* destFilename) {
        if (!_getAlphaFreq(srcFilename)) return false;
        huffTree htree(_afMap);
        htree.huffmanCode(_codeMap);
        return _encode(srcFilename, destFilename);
    }
private:
    int _getLastValidBit() {
        int sum = 0;
        for (auto it : _codeMap) {
            sum += it.second.size() * _afMap.at(it.first);
            sum &= 0xFF;
        }
        sum &= 0x7;
        return sum == 0 ? 8 : sum;
    }
    bool _getAlphaFreq(const char* filename) {
        uchar ch;
        std::ifstream is(filename, std::ios::binary);
        if (!is.is_open()) {
            printf("read file failed! filename: %s", filename);
            return false;
        }
        is.read((char*)&ch, sizeof(uchar));
        while (!is.eof()) {
            _afMap[ch]++;
            is.read((char*)&ch, sizeof(uchar));
        }
        is.close();
        return true;
    }
    bool _encode(const char* srcFilename, const char* destFilename) {
        uchar ch;
        uchar value;
        int bitIndex = 0;
        fileHead filehead = { 'e', 'v', 'e', 'n' };
        filehead.alphaVariety = (uchar)_afMap.size();
        filehead.lastValidBit = _getLastValidBit();

        std::ifstream is(srcFilename, std::ios::binary);
        if (!is.is_open()) {
            printf("read file failed! filename: %s", srcFilename);
            return false;
        }
        std::ofstream io(destFilename, std::ios::binary);
        if (!io.is_open()) {
            printf("read file failed! filename: %s", destFilename);
            return false;
        }

        io.write((char*)&filehead, sizeof(fileHead));
        for (auto i : _afMap) {
            alphaFreq af(i);
            io.write((char*)&af, sizeof(alphaFreq));
        }

        is.read((char*)&ch, sizeof(uchar));
        while (!is.eof()) {
            std::string code = _codeMap.at(ch);
            for (auto c : code) {
                if ('0' == c) {
                    CLR_BYTE(value, bitIndex);
                }
                else {
                    SET_BYTE(value, bitIndex);
                }
                ++bitIndex;
                if (bitIndex >= 8) {
                    bitIndex = 0;
                    io.write((char*)&value, sizeof(uchar));
                }
            }
            is.read((char*)&ch, sizeof(uchar));
        }

        if (bitIndex) {
            io.write((char*)&value, sizeof(uchar));
        }
        is.close();
        io.close();
        return true;
    }
private:
    std::map<uchar, int> _afMap;
    std::map<uchar, std::string> _codeMap;
};


class huffDecode {
public:
    huffDecode() : _fileHead(nullptr), _htree(nullptr) {
        _fileHead = new fileHead();
    }
    ~huffDecode() {
        if (!_fileHead) delete _fileHead;
        if (!_htree) delete _htree;
    }
private:
    static bool _isLeaf(Node* n) {
        return n->left == nullptr && n->right == nullptr;
    }
    long _getFileSize(const char* strFileName) {
        std::ifstream in(strFileName);
        if (!in.is_open()) return 0;

        in.seekg(0, std::ios_base::end);
        std::streampos sp = in.tellg();
        in.close();
        return sp;
    }
    bool _getAlphaFreq(const char* filename) {
        std::ifstream is(filename, std::ios::binary);
        if (!is) {
            printf("read file failed! filename: %s", filename);
            return false;
        }

        is.read((char*)_fileHead, sizeof(fileHead));
        if (!(_fileHead->flag[0] == 'e' &&
            _fileHead->flag[1] == 'v' &&
            _fileHead->flag[2] == 'e' &&
            _fileHead->flag[3] == 'n')) {
            printf("not support this file format! filename: %s\n", filename);
            return false;
        }
        for (int i = 0; i < static_cast<int>(_fileHead->alphaVariety); ++i) {
            alphaFreq af;
            is.read((char*)&af, sizeof(af));
            _afMap.insert(std::pair<char, int>(af.alpha, af.freq));
        }
        is.close();
        return true;
    }
    bool _decode(const char* srcFilename,
        const char* destFilename) {
        long fileSize = _getFileSize(srcFilename);

        std::ifstream is(srcFilename, std::ios::binary);
        if (!is) {
            printf("read file failed! filename: %s", srcFilename);
            return false;
        }
        is.seekg(sizeof(fileHead) + sizeof(alphaFreq) * _fileHead->alphaVariety);

        Node node = _htree->getHuffTree();
        Node* pNode = &node;

        std::ofstream io(destFilename, std::ios::binary);
        if (!io) {
            printf("create file failed! filename: %s", destFilename);
            return false;
        }

        uchar value;
        std::string code;
        int index = 0;
        long curLocation = is.tellg();
        is.read((char*)&value, sizeof(uchar));
        while (1) {
            if (_isLeaf(pNode)) {
                uchar alpha = _decodeMap[code];
                io.write((char*)&alpha, sizeof(uchar));
                if (curLocation >= fileSize && index >= _fileHead->lastValidBit) {
                    break;
                }
                code.clear();
                pNode = &node;
            }

            if (GET_BYTE(value, index)) {
                code += '1';
                pNode = pNode->right;
            }
            else {
                pNode = pNode->left;
                code += '0';
            }
            if (++index >= 8) {
                index = 0;
                is.read((char*)&value, sizeof(uchar));
                curLocation = is.tellg();
            }
        }

        is.close();
        io.close();
        return true;
    }
public:
    bool decode(const char* srcFilename, const char* destFilename) {
        if (!_getAlphaFreq(srcFilename)) return false;
        long fileSize = _getFileSize(srcFilename);
        _htree = new huffTree(_afMap);
        _htree->watch();
        _htree->huffmanCode(_codeMap);

        for (auto it : _codeMap) {
            _decodeMap.insert(std::pair<std::string, uchar>(it.second, it.first));
        }

        return _decode(srcFilename, destFilename);
    }
private:
    fileHead* _fileHead;
    huffTree* _htree;
    std::map<uchar, int> _afMap;
    std::map<uchar, std::string> _codeMap;
    std::map<std::string, uchar> _decodeMap;
};


inline void _Huffman_Encode(const std::string& filename) {
    std::string destFilename = "en" + filename;
    huffEncode he;
    he.encode(filename.data(), destFilename.data());
}

inline void _Huffman_Decode(const std::string& filename) {
    std::string destFilename = "de" + filename;
    huffDecode hd;
    hd.decode(filename.data(), destFilename.data());
}

//}// huffman namespace
