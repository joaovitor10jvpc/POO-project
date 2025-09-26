#pragma once
#include <stack>
#include <memory>

namespace agenda {

class ICommand {
public:
    virtual ~ICommand() = default;
    virtual void execute() = 0;
    virtual void undo() = 0;
};

class UndoRedoStack {
    std::stack<std::unique_ptr<ICommand>> undoStack_;
    std::stack<std::unique_ptr<ICommand>> redoStack_;
public:
    void perform(std::unique_ptr<ICommand> cmd) {
        cmd->execute();
        undoStack_.push(std::move(cmd));
        while(!redoStack_.empty()) redoStack_.pop();
    }
    void undo() {
        if(undoStack_.empty()) return;
        auto cmd = std::move(undoStack_.top()); undoStack_.pop();
        cmd->undo();
        redoStack_.push(std::move(cmd));
    }
    void redo() {
        if(redoStack_.empty()) return;
        auto cmd = std::move(redoStack_.top()); redoStack_.pop();
        cmd->execute();
        undoStack_.push(std::move(cmd));
    }
};

} // namespace agenda
