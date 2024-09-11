#include "../../ASTNodes.h"

#include "../../../tokens/TokenSet.h"
#include "../../../tokens/token.h"
#include "NBlock.h"
#include "NVariableDeclaration.h"

using namespace stride::ast;

/**
 * Try catch statement.
 * Try catch statements are used to catch exceptions.
 * They are defined using the following format: <br />
 * <code>
 * try { <br />
 *   &nbsp;&lt;statement&gt; ... <br />
 *  } catch (err: Type) { <br />
 *   &nbsp;&lt;statement&gt; ... <br />
 *  }
 */
class NTryCatchStatement : public Node
{
private:
    NBlock *tryBlock;
    NBlock *catchBlock;
    NVariableDeclaration *exception;

public:
    NTryCatchStatement(NBlock *try_block, NBlock *catch_block) :
            tryBlock(try_block),
            catchBlock(catch_block),
            exception(nullptr)
    {}

    /**
     * Set the exception variable.
     * @param exception The exception variable.
     */
    void setException(NVariableDeclaration *exception)
    {
        this->exception = exception;
    }

    /**
     * Set the try block.
     * This is the block that will be executed first,
     * and will be the block that could throw an exception.
     * @param try_block The try block.
     */
    void setTryBlock(NBlock *try_block)
    {
        this->tryBlock = try_block;
    }

    /**
     * Set the catch block.
     * @param catch_block The catch block.
     */
    void setCatchBlock(NBlock *catch_block)
    {
        this->catchBlock = catch_block;
    }

    enum ENodeType getType() override
    { return TRY_CATCH_CLAUSE; }

    static void parse(TokenSet &tokenSet, Node &parent);

};