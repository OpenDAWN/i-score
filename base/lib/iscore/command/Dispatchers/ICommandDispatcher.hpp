#pragma once
namespace iscore
{
    class CommandStack;
}

/**
 * @brief The ICommandDispatcher class
 *
 * Base for command dispatchers.
 * Command dispatchers are utility class to send many commands in a specific
 * way.
 *
 * The general interface is :
 *   - submitCommand(cmd) to send a new command or update an existing one
 *   - commit() when editing is done.
 */
class ICommandDispatcher
{
    public:
        ICommandDispatcher(iscore::CommandStack& stack):
            m_stack{stack}
        {

        }

        iscore::CommandStack& stack() const
        {
            return m_stack;
        }

    private:
        iscore::CommandStack& m_stack;
};
