
#pragma once
#include <L3.h>
#include <stdint.h>
#include <instructionSelect.h>

namespace L3 {
    
    class CodeGen;
    class TileRule {
        public:
        virtual bool verify(TreeNode *tree) = 0;
    };

    class TileRule_one: public TileRule {
        public:
        bool verify(TreeNode *tree) override;
    };

    class TileRule_1248: public TileRule {
        public:
        bool verify(TreeNode *) override;
    };

    class TileNode {
#define TileNodeTypeNumber 0x0001
#define TileNodeTypeVariable 0x0002
#define TileNodeTypeLabel 0x0004
#define TileNodeTypeEmpty 0x0008
#define TileNodeTypeOp 0x0010

        public:
        TreeNode *matched_node = nullptr;
        unsigned short tile_type = 0;
        int64_t id = 0;
        Operation *op = nullptr;
        bool left = true;
        bool noflip = false;
        
        TileNode *oprand1 = nullptr;
        TileNode *oprand2 = nullptr;
        bool match(TreeNode *);
        bool isLeaf();
        TileRule *rule = nullptr;
    };

    class Tile {
        public:
        std::string name;
        // Tile();
        /**
         * @brief check if current tile matches a tree.
         * 
         * @param subtrees if the match process is successful, this is set to
         * be a list of subtrees. do not use this when return value is false.
         * @return true if matching is successful
         * @return false unsuccessful
         */
        bool match(TreeNode *, set<TreeNode *> &subtrees);
        int64_t getSize();
        // void accept();
        // vector<L2::Instruction *> getInstructions();
        virtual Tile *clone() = 0;
        virtual void accept(CodeGen *) = 0;
        TreeNode *getTree() { return root->matched_node; }
        TileNode *root;
        
        private:
        int64_t size = -1;
        // vector<L2::Instruction *> instructions;
    };

    class Tile_math: public Tile {
        public:
        Tile_math(std::string op);
        Tile *clone() override;
        void accept(CodeGen *g) override;
    };

    class Tile_compare: public Tile {
        public:
        Tile_compare(std::string op);
        Tile *clone() override;
        void accept(CodeGen *g) override;
    };

    class Tile_assign: public Tile {
        public:
        Tile_assign();
        Tile *clone() override;
        void accept(CodeGen *g) override;
    };

    class Tile_load: public Tile {
        public:
        Tile_load();
        Tile *clone() override;
        void accept(CodeGen *g) override;
    };

    class Tile_store: public Tile {
        public:
        Tile_store();
        Tile *clone() override;
        void accept(CodeGen *g) override;
    };

    class Tile_br: public Tile {
        public: 
        Tile_br();
        Tile *clone() override;
        void accept(CodeGen *g) override;
    };

    class Tile_br_t: public Tile {
        public: 
        Tile_br_t();
        Tile *clone() override;
        void accept(CodeGen *g) override;
    };

    class Tile_return : public Tile {
        public:
        Tile_return();
        Tile *clone() override;
        void accept(CodeGen *g) override;
    };

    class Tile_return_t : public Tile {
        public:
        Tile_return_t();
        Tile *clone() override;
        void accept(CodeGen *g) override;
    };

    /**
     * @brief *= += -= <<= >>= &=
     * 
     */
    class Tile_math_specialized: public Tile {
        public:
        Tile_math_specialized(std::string);

        Tile *clone() override;
        void accept(CodeGen *g) override;
    };

    class Tile_increment: public Tile {
        public:
        bool inc_or_dec;
        Tile_increment(bool inc_or_dec);
        Tile *clone() override;
        void accept(CodeGen *g) override;
    };

    class Tile_at: public Tile {
        public:
        Tile_at();
        Tile *clone() override;
        void accept(CodeGen *g) override;
    };

    /**
     * @brief do tiling using biggest munch
     * 
     * @param root the root of the tree to be tiled
     * @param res the result
     */
    void tiling(TreeNode *root, vector<Tile *>&res, const vector<Tile *> all_tiles);
    vector<Tile *> getAllTiles();
}