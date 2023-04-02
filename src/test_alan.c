extern int WIP;
#include "kdtree.h"
#include "queue.h"
#include "str_util.h"
#include <stb_ds.h>

Queue* worldTreeQueue; // ongoing generating of the worlds nodes
Queue* worldTreeExtant; // finished nodes
Queue* worldTreeExtinct; // culled nodes that may still be usefull

typedef struct Replacement {
    // world.txt --> top 50 room garden
    bool orientation; // top (true), left (false)
    uint8_t splitPercent;
    uint32_t type1;
    uint32_t type2;
} Replacement;

typedef struct NodeType {
    uint32_t type;
    vec3* col;
    Replacement** replacements; // Replacement pointer array
    char* typeName; // for use only when reading world.txt
} NodeType;

char* readWorld()
{
    char* const buffer;
    {
        char path = "world.txt";
        FILE* const file = fopen(path, "rb");
        if (!file) {
            fprintf(stderr, "Failed to fopen ");
            perror(path);
            return 0;
        }
        fseek(file, 0L, SEEK_END);
        const long file_size = ftell(file);
        rewind(file);
        char* const buffer = calloc(1, file_size + 1);
        if (fread(buffer, file_size, 1, file) != 1)
        {
            fclose(file);
            free(buffer);
            fprintf(stderr, "Failed to fread for %s\n", path);
            return 0;
        }
        fclose(file);
    }
    return buffer;
}

// reads and returns up to 2^32 node types from world.txt
// they are delimited by an empty line
// each node type in world.txt looks like this
//
// world
// 0 80 0
// top 50 room garden
// top 50 garden garden
NodeType** readRules()
{
    char* const file = readWorld();
    bool ok = true;
    uint32_t fileIndexStart;
    fileIndexStart = strcmp(string_substr(file, 0, 19), "code starts here:\n\n") ? 19 :
        strcmp(string_substr(file, 0, 24), "---\n\ncode starts here") ? 24 :
        string_skipUntilThenSkip(file, "\n---\n\ncode starts here:\n\n", 0);
    uint32_t fileIndex = fileIndexStart;
    NodeType* types = NULL; // nodeType array

    // start again at the start of the file to fill in "types"
    while (file[fileIndex] != '\0') { // until end of file
        uint32_t prevFileIndex; // for comparison when I skip parts
        arrput(types, ((NodeType) {.type = 0}));

        // read typename
        prevFileIndex = string_skipUntilWhitespace(file, fileIndex);
        for (uint32_t nodeIndex = 0; nodeIndex < arrlenu(types); nodeIndex++) {
            if (strcmp(types[nodeIndex].typeName,
                string_substr(file, fileIndex, prevFileIndex - fileIndex))) {
                arrlast(types).type = nodeIndex;
            }
        }
        fileIndex = string_skipWhitespace(file, prevFileIndex);

        // read col
        for (uint8_t n = 0; n < 3; n++) {
            vec3 col;
            for (uint8_t channel = 0; channel < 2; channel++) {
                prevFileIndex = string_skipUntilWhitespace(file, fileIndex);
                col[channel] = strtof(&(file[fileIndex]), &(file[prevFileIndex]));
                fileIndex = string_skipWhitespace(file, prevFileIndex);
            }
            arrlast(types).col = col;
        }

        prevFileIndex = fileIndex;
        uint32_t replacementCount = 0;
        while (fileIndex == prevFileIndex) // until double line break keep reading replacement rule lines
        {
            // read oriention
            char* substr = string_substr(file, fileIndex, 0);
            arrlast(types).replacements[replacementCount]->orientation = strcmp(substr, "left");
            free(substr);
            fileIndex++;

            // read proportion
            prevFileIndex = string_skipUntilWhitespace(file, fileIndex);
            arrlast(types).replacements[replacementCount]->splitPercent =
                strtof(&(file[fileIndex]), &(file[prevFileIndex]));
            fileIndex++;

            for (uint8_t n = 0; n < 2; n++) {
                // read typename
                prevFileIndex = string_skipUntilWhitespace(file, fileIndex);
                for (uint32_t nodeIndex = 0; nodeIndex < arrlenu(types); nodeIndex++) {
                    if (strcmp(arrlast(types).typeName,
                        string_substr(file, fileIndex, prevFileIndex - fileIndex))) {
                        arrlast(types).type = nodeIndex;
                    }
                }
                prevFileIndex = string_skipLineBreak(file, prevFileIndex);
                fileIndex = string_skipLineBreak(file, prevFileIndex);
            }

            replacementCount++;
        }
    };

    return types;
}

void processQueue(Queue* self)
{
    uint16_t iters = 100;

    NodeType* types = readRules(); // reading in node types text file

    uint16_t iter = 0;
    while (iter < iters && !queue_isEmpty(self)) {
        iter++;
        Node* next = queue_pop(self); // get the front item of the queue
        node_birth(types, next); // generate two children to add to the back of the queue
        Node** children = next->children;
        bool ok = true;
        for (uint8_t p = 0; p < 1; p++) {
            for (uint8_t q = 0; q < 1; q++) {
                if (children[p]->size[q] < 0.001) ok = false;
            }
        }
        if (ok) {
            queue_push(self, next->children[0]);
            queue_push(self, next->children[1]);
        }
        queue_push(worldTreeExtant, next); // add to the queue collection of finished
    }

    // result as 2D array, or 1D array of rectangle
}

int main(int argc, char* argv[])
{
    worldTreeQueue = queue();
    worldTreeExtant = queue();
    worldTreeExtinct = queue();
    processQueue(worldTreeQueue);
    /*
    Node* n1 = malloc(sizeof(Node));
    Queue* c1 = malloc(sizeof(Queue));
    c1 = queue();
    //queue_push(c1, n1);
    printf("%d\n", (int)queue_len_blocks(c1));
    printf("%d\n", (int)queue_len_items(c1));
    //getchar();
    printf("\ndone");
    */
}
