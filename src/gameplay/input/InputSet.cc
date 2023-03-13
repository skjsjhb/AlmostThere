#include "InputSet.hh"

#include "engine/virtual/Input.hh"

void InputSet::pollInputEvents()
{
    vtSetActiveInputSet(this);
    vtPollEvents();
};

void InputSet::setupInputListeners()
{
    vtSetupKeyListener();
}