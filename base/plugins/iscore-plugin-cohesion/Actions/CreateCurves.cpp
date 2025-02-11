#include "CreateCurves.hpp"

#include <Commands/CreateCurvesFromAddresses.hpp>
#include <Commands/CreateCurvesFromAddressesInConstraints.hpp>

#include <Scenario/Document/Constraint/ConstraintModel.hpp>
#include <Explorer/Explorer/DeviceExplorerModel.hpp>

#include <iscore/command/Dispatchers/MacroCommandDispatcher.hpp>

#include <core/document/Document.hpp>

void CreateCurves(iscore::Document& doc)
{
    using namespace std;
    // Fetch the selected constraints
    auto sel = doc.
            selectionStack().
            currentSelection();

    QList<const ConstraintModel*> selected_constraints;
    for(auto obj : sel)
    {
        if(auto cst = dynamic_cast<const ConstraintModel*>(obj.data()))
            if(cst->selection.get())
                selected_constraints.push_back(cst);
    }

    // Fetch the selected DeviceExplorer elements
    auto& device_explorer = deviceExplorerFromObject(doc);
    auto addresses = device_explorer.selectedIndexes();

    MacroCommandDispatcher macro{new CreateCurvesFromAddressesInConstraints,
                doc.commandStack()};
    for(auto constraint : selected_constraints)
    {
        QList<iscore::Address> l;
        for(const auto& index : addresses)
        {
            l.push_back(iscore::address(device_explorer.nodeFromModelIndex(index)));
        }

        // TODO skip the ones that can't send messages or aren't int / double / float
        auto cmd = new CreateCurvesFromAddresses {*constraint, l};
        macro.submitCommand(cmd);
    }

    macro.commit();
}
