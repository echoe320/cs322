
#include <tiling.h>
#include <codegenerator.h>
#include <iostream>
#include <queue>

using namespace std;

extern bool is_debug;
namespace L3 {

    bool TileRule_one::verify(TreeNode *tree) {
        Number *n = dynamic_cast<Number *>(tree->val);
        if (n) {
            return n->get() == 1;
        } else {
            return false;
        }
    }

    bool TileRule_1248::verify(TreeNode *tree) {
        Number *n = dynamic_cast<Number *>(tree->val);
        if (n) {
            return n->get() == 1 || n->get() == 2 || n->get() == 4 || n->get() == 8;
        } else {
            return false;
        }
    }

    bool TileNode::isLeaf() {
        return oprand1 == nullptr && oprand2 == nullptr;
    }

    bool TileNode::match(TreeNode *tree) {
        if (!tree) {
            return false;
        }
        if (op) {
            if (!tree->op) {
                return false;
            } else if (tree->op->toString() != op->toString()) {
                return false;
            }
        }
        if (rule && !rule->verify(tree)) {
            return false; // it obeys our rule
        }
        ItemType t = tree->val->getType();
        switch (t) {
        case item_label:
            return tile_type & TileNodeTypeLabel;
        case item_number:
            return tile_type & TileNodeTypeNumber;
        case item_variable:
            return tile_type & TileNodeTypeVariable;
        case item_empty:
            return tile_type & TileNodeTypeEmpty;
        case item_operation:
            return tile_type & TileNodeTypeOp;
        default:
            return false;
            break;
        }
    }

    bool match_helper(TileNode *tile, TreeNode *tree,
            std::map<pair<short, int64_t>, TreeNode *> &nodemap) {
        if (tile == nullptr) {
            return true; // tile is null, it matches any tree
        }        
        if (tree == nullptr) {
            if (is_debug) cout << "tree pt is null" << endl;
            return false; // tile has this node, tree doen't have
        }
        bool curr = tile->match(tree); // match op and type
        if (!curr) {
            // if (is_debug) cout << "op or type not match" << endl;
            return false;
        }


        bool mapchaged = false;
        // if this item is required to be the same as previous variables
        if (nodemap.count({tile->tile_type, tile->id}) != 0) {
            if (tree->val != nodemap[{tile->tile_type, tile->id}]->val) {
                if (is_debug) cout << "not same variable" << endl;
                return false; // different from previous stored variable
            }
        } else {
            // haven't seen it yet, store it
            mapchaged = true;
            nodemap[{tile->tile_type, tile->id}] = tree;
        }

        auto nodemap_clone = nodemap;

        // if (tile->isLeaf() && (!tree->isroot)) {
        //     if (is_debug) cout << "adding: " << tree->val->toString() << endl;
        //     subtrees.insert(tree);
        // }
        bool left = match_helper(tile->oprand1, tree->oprand1, nodemap);
        bool right = match_helper(tile->oprand2, tree->oprand2, nodemap);

        if (left && right) {
            tile->matched_node = tree;
            return true;
        }

        if (tile->noflip) {
            if (mapchaged)
                nodemap.erase({tile->tile_type, tile->id});
            return false;
        }

        nodemap = nodemap_clone; // restore

        left = match_helper(tile->oprand1, tree->oprand2, nodemap);
        right = match_helper(tile->oprand2, tree->oprand1, nodemap);
        if (left && right) {
            tile->matched_node = tree;
            return true;
        }
        
        if (mapchaged) {
            nodemap.erase({tile->tile_type, tile->id});
        }
        return false;
    }

    void getSubTrees(TileNode *root, set<TreeNode *> &subtrees) {
        if (root == 0) {
            return;
        }

        if (root->isLeaf() && (!root->matched_node->isroot)) {
            if (is_debug) cout << "adding " << root->matched_node->val->toString() << endl;
            subtrees.insert(root->matched_node);
        } else {
            getSubTrees(root->oprand1, subtrees);
            getSubTrees(root->oprand2, subtrees);
        }
        return;
    }

    bool Tile::match(TreeNode *t, set<TreeNode *> &subtrees) {
        std::map<pair<short, int64_t>, TreeNode *> nodemap;
        // cout << "tile name :" << name << endl;
        bool matched = match_helper(root, t, nodemap);
        if (matched) getSubTrees(root, subtrees);
        return matched;
    }

    int64_t Tile::getSize() {
        if (size == -1) {
            queue<TileNode *> tileque;
            tileque.push(this->root);

            size = 0;
            while (!tileque.empty()) {
                TileNode *currtile = tileque.front();
                tileque.pop();
                size++;
                if (currtile->oprand1) {
                    tileque.push(currtile->oprand1);
                }
                if (currtile->oprand2) {
                    tileque.push(currtile->oprand2);
                }
            }
        }
        return size;
    }
    Tile_math::Tile_math(std::string op) {
        name = "tile_math";
        this->root = new TileNode();
        root->id = 0;
        root->tile_type |= TileNodeTypeVariable;
        root->op = new Operation(op);
        root->oprand1 = new TileNode();
        root->oprand1->tile_type |= TileNodeTypeNumber;
        root->oprand1->tile_type |= TileNodeTypeVariable;
        root->oprand1->id = 1;
        root->oprand2 = new TileNode();
        root->oprand2->tile_type |= TileNodeTypeNumber;
        root->oprand2->tile_type |= TileNodeTypeVariable;
        root->oprand2->id = 2;
    }
    Tile* Tile_math::clone() {
        return new Tile_math(root->op->toString()); 
    }

    Tile_compare::Tile_compare(std::string op) {
        name = "tile_compare";
        this->root = new TileNode();
        root->id = 0;
        root->tile_type |= TileNodeTypeVariable;
        root->op = new Operation(op);
        root->oprand1 = new TileNode();
        root->oprand1->tile_type |= TileNodeTypeNumber;
        root->oprand1->tile_type |= TileNodeTypeVariable;
        root->oprand1->id = 1;
        root->oprand2 = new TileNode();
        root->oprand2->tile_type |= TileNodeTypeNumber;
        root->oprand2->tile_type |= TileNodeTypeVariable;
        root->oprand2->id = 2;
    }
    Tile* Tile_compare::clone() {
        return new Tile_compare(root->op->toString());
    }

    Tile_math_specialized::Tile_math_specialized(std::string op) {
        name = "tile_math_specialized";
        this->root = new TileNode();
        root->id = 0;
        root->tile_type |= TileNodeTypeVariable;
        if (op == "-" || op == "<<" || op == ">>") {
            root->noflip = true;
        }
        root->op = new Operation(op);
        root->oprand1 = new TileNode();
        root->oprand2 = new TileNode();
        root->oprand1->tile_type |= TileNodeTypeVariable;
        root->oprand1->id = 0;
        root->oprand2->tile_type |= TileNodeTypeNumber;
        root->oprand2->tile_type |= TileNodeTypeVariable;
        root->oprand2->id = 1;
    }
    Tile* Tile_math_specialized::clone() {
        return new Tile_math_specialized(root->op->toString()); 
    }
    Tile_assign::Tile_assign() {
        name = "tile_assign";
        root = new TileNode();
        root->op = new Operation("<-");
        root->id = 0;
        root->tile_type |= TileNodeTypeVariable;
        root->oprand1 = new TileNode();
        root->oprand1->tile_type |= TileNodeTypeVariable;
        root->oprand1->tile_type |= TileNodeTypeLabel;
        root->oprand1->tile_type |= TileNodeTypeNumber;
        root->id = 1;
    }

    Tile* Tile_assign::clone() {
        return new Tile_assign();
    }

    Tile_load::Tile_load() {
        name = "tile_load";
        root = new TileNode();
        root->op = new Operation("load");
        root->id = 0;
        root->tile_type |= TileNodeTypeVariable;
        root->oprand1 = new TileNode();
        root->oprand1->tile_type |= TileNodeTypeVariable;
        root->oprand1->id = 1;
    }

    Tile* Tile_load::clone() {
        return new Tile_load();
    }
    Tile_store::Tile_store() {
        name = "tile_store";
        root = new TileNode();
        root->op = new Operation("store");
        root->id = 0;
        root->tile_type |= TileNodeTypeVariable;
        root->oprand1 = new TileNode();
        root->oprand1->tile_type |= TileNodeTypeVariable;
        root->oprand1->tile_type |= TileNodeTypeLabel;
        root->oprand1->tile_type |= TileNodeTypeNumber;
        root->oprand1->id = 1;
    }
    Tile* Tile_store::clone() {
        return new Tile_store();
    }

    Tile_br::Tile_br() {
        name = "tile_br";
        root = new TileNode(); 
        root->op = new Operation("br"); 
        root->id = 0; 
        root->tile_type |= TileNodeTypeEmpty; 
        root->oprand1 = new TileNode(); 
        root->oprand1->tile_type |= TileNodeTypeLabel;
        root->oprand1->id = 1; 
    }
    Tile* Tile_br::clone() {
        return new Tile_br();
    }

    Tile_br_t::Tile_br_t() {
        name = "tile_br_t";
        root = new TileNode(); 
        root->op = new Operation("br"); 
        root->id = 0; 
        root->tile_type |= TileNodeTypeEmpty; 
        root->oprand1 = new TileNode(); 
        root->oprand1->tile_type |= TileNodeTypeVariable;
        root->oprand1->tile_type |= TileNodeTypeNumber;
        root->oprand1->id = 1; 
        root->oprand2 = new TileNode(); 
        root->oprand2->tile_type |= TileNodeTypeLabel;
        root->oprand2->id = 2; 
    }
    Tile* Tile_br_t::clone() {
        return new Tile_br_t();
    } 

    Tile_return::Tile_return() {
        name = "tile_return";
        root = new TileNode(); 
        root->op = new Operation("return");
        root->id = 0; 
        root->tile_type |= TileNodeTypeEmpty;
    }
    Tile* Tile_return::clone() {
        return new Tile_return();
    } 
    Tile_return_t::Tile_return_t() {
        name = "tile_return_t";
        root = new TileNode(); 
        root->op = new Operation("return");
        root->id = 0; 
        root->tile_type |= TileNodeTypeEmpty;
        root->oprand1 = new TileNode(); 
        root->oprand1->tile_type |= TileNodeTypeVariable; 
        root->oprand1->tile_type |= TileNodeTypeNumber;
        root->oprand1->id = 1;   
    }      
    Tile* Tile_return_t::clone() {
        return new Tile_return_t();
    } 

    Tile_increment::Tile_increment(bool is_increment) {
        name = "tile_increment";
        this->inc_or_dec = is_increment;
        root = new TileNode();
        root->id = 0;
        root->tile_type |= TileNodeTypeVariable;
        TileNode *var, *num;
        if (is_increment) {
            root->op = new Operation("+");
        } else {
            root->op = new Operation("-");
        }
        var = new TileNode();
        var->tile_type |= TileNodeTypeVariable;
        var->id = 0;
        num = new TileNode();
        num->tile_type |= TileNodeTypeNumber;
        num->rule = new TileRule_one();
        num->id = 1;

        // if (left) {
        root->oprand1 = var;
        root->oprand2 = num;
        // } else {
        //     root->oprand2 = var;
        //     root->oprand1 = num;
        // }
    }
    Tile* Tile_increment::clone() { return new Tile_increment(inc_or_dec); }
    Tile_at::Tile_at() {
        name = "tile_at";
        root = new TileNode();
        root->id = 0;
        root->op = new Operation("+");
        root->tile_type |= TileNodeTypeVariable;

        root->oprand1 = new TileNode();
        root->oprand1->op = new Operation("*");
        root->oprand1->tile_type |= TileNodeTypeVariable;
        root->oprand1->id = 1;

        root->oprand2 = new TileNode();
        root->oprand2->tile_type |= TileNodeTypeVariable;
        root->oprand2->id = 2;

        root->oprand1->oprand1 = new TileNode();
        root->oprand1->oprand1->tile_type |= TileNodeTypeVariable;
        root->oprand1->oprand1->id = 3;

        root->oprand1->oprand2 = new TileNode();
        root->oprand1->oprand2->tile_type |= TileNodeTypeNumber;
        root->oprand1->oprand2->id = 4;
        root->oprand1->oprand2->rule = new TileRule_1248();
    }
    Tile* Tile_at::clone() { return new Tile_at(); }

    void Tile_math::accept(CodeGen *g) {
        g->visit(this);
    }

    void Tile_compare::accept(CodeGen *g) {
        g->visit(this);
    }

    void Tile_assign::accept(CodeGen *g) {
            g->visit(this);
    }

    void Tile_load::accept(CodeGen *g) {
            g->visit(this);
    }

    void Tile_store::accept(CodeGen *g) {
            g->visit(this);
    }

    void Tile_br::accept(CodeGen *g) {
            g->visit(this);
    }

    void Tile_br_t::accept(CodeGen *g) {
            g->visit(this);
    }

    void Tile_return ::accept(CodeGen *g) {
            g->visit(this);
    }

    void Tile_return_t ::accept(CodeGen *g) {
            g->visit(this);
    }

    /**
     * @brief *= += -= <<= >>= &=
     * 
     */
    void Tile_math_specialized::accept(CodeGen *g) {
            g->visit(this);
    }

    void Tile_increment::accept(CodeGen *g) {
            g->visit(this);
    }

    void Tile_at::accept(CodeGen *g) {
            g->visit(this);
    }


    void tiling(TreeNode *root, vector<Tile *>&res, const vector<Tile *> all_tiles) {
        bool flag = false;
        if (root == nullptr) {
            return;
        }
        // if (root->oprand1 == nullptr && root->oprand2 == nullptr) {
        //     //if it is a return
        //     // vector<TreeNode *> subtrees;
        //     // Tile_return* r = new Tile_return(); 
        //     // if(r->match(root, subtrees)) {
        //     //     flag = true;
        //     //     cout << "matched to tile: " << r->name << endl;
        //     //     root->printNode(root, 0);
        //     //     res.push_back(r);
        //     return; // a leaf node skip
        //     // }
        // }
        for (auto t :all_tiles) {
            set<TreeNode *> subtrees;
            Tile *t_clone = t->clone();
            // cout << "tring to match to: " << t->name << endl;
            if (t_clone->match(root, subtrees)) {
                flag = true;
                if(is_debug) cout << "matched to tile: " << t->name << endl;
                // cout << "subtreesize: " << subtrees.size() << endl;
                res.push_back(t_clone);
                for (auto sub: subtrees) {
                    tiling(sub, res, all_tiles);
                }
                break;
            }
            //delete t_clone;
        }

        if (!flag) {
            cerr << "warning: cannot match to any tile." << endl;
        }
        return;
    }

    
    vector<Tile *> getAllTiles() {
        Tile *at = new Tile_at();
        Tile *inc_left = new Tile_increment("+");
        // Tile *inc_right = new Tile_increment("+", false);
        Tile *dec_left = new Tile_increment("-");
        // Tile *dec_right = new Tile_increment("-", false);
        Tile *plus_left = new Tile_math_specialized("+");
        // Tile *plus_right = new Tile_math_specialized("+", false);

        Tile *minus_left = new Tile_math_specialized("-");
        // Tile *minus_right = new Tile_math_specialized("-", false);
        Tile *mul_left = new Tile_math_specialized("*");
        // Tile *mul_right = new Tile_math_specialized("*", false);
        Tile *and_left = new Tile_math_specialized("&");
        // Tile *and_right = new Tile_math_specialized("&", false);
        Tile *sl_left = new Tile_math_specialized("<<");
        // Tile *sl_right = new Tile_math_specialized("<<", false);
        Tile *sr_left = new Tile_math_specialized(">>");
        // Tile *sr_right = new Tile_math_specialized(">>", false);
        Tile *plus_g = new Tile_math("+");
        Tile *minus_g = new Tile_math("-");
        Tile *mul_g = new Tile_math("*");
        Tile *and_g = new Tile_math("&");
        Tile *sl_g = new Tile_math("<<");
        Tile *sr_g = new Tile_math(">>");

        Tile *equal = new Tile_compare("=");
        Tile *greater = new Tile_compare(">");
        Tile *less = new Tile_compare("<");
        Tile *ge = new Tile_compare(">=");
        Tile *le = new Tile_compare("<=");

        Tile *ass = new Tile_assign();
        Tile *load = new Tile_load();
        Tile *store = new Tile_store();
        Tile *ret = new Tile_return(); 
        Tile *ret_t = new Tile_return_t();
        Tile *br = new Tile_br(); 
        Tile *br_t = new Tile_br_t();

        vector<Tile *> all_tiles = {
            at,
            inc_left,
            dec_left,
            plus_left,
            // plus_right,
            minus_left,
            // minus_right,
            mul_left,
            // mul_right,
            and_left,
            // and_right,
            sl_left,
            // sl_right,
            sr_left,
            // sr_right,
            plus_g,
            minus_g,
            mul_g,
            and_g,
            sl_g,
            sr_g,
            equal,
            greater,
            less,
            ge,
            le,
            ass,
            load,
            store,
            ret_t,
            ret,
            br_t,
            br,
        };

        return all_tiles;
    }

}