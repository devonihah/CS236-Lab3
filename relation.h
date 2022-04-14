#pragma once
#include <vector>
#include <string>
#include <set>
#include <list>
#include "scheme.h"
#include "tuple.h"

using namespace std;

class relation {

private:

    string id;
    scheme schemes;
    set<tuples> tuplesSet;

public:
    relation() = default;
    relation(string name, scheme scheme) : id(name), schemes(scheme) { }
    string getID() { return id; }
    void setID(string newID) { id = newID; }
    scheme getScheme() { return schemes; }
    void setScheme(scheme newScheme) { schemes = newScheme; }
    void addTuple(const tuples& tupleToAdd) { tuplesSet.insert(tupleToAdd); }
    void setTuples(set<tuples> tuples) { tuplesSet = tuples; }
    size_t tuplesSetSize() { return tuplesSet.size(); }

    relation select(int index1, int index2)
    {
        relation newRelation(id, schemes);
        for (tuples it : tuplesSet)
        {
            if (it.getValues()[index1] == it.getValues()[index2])
            {
                newRelation.addTuple(it);
            }
        }
        return newRelation;
    }

    relation select(int index, const string& value) const {
        relation newRelation(id, schemes);
        for (auto tuple : tuplesSet)
            if (tuple.at(index) == value)
                newRelation.addTuple(tuple);
        return newRelation;
    }

    relation project(vector<int> indices)
    {
        vector<string> newAttributes;
        for (int it : indices)
        {
            newAttributes.push_back(schemes.getAttributes()[it]);
        }
        scheme newScheme(newAttributes);
        relation newRelation(id, newScheme);

        for (tuples it : tuplesSet)
        {
            vector<string> newValues;
            for (int ti : indices)
            {
                newValues.push_back(it.getValues()[ti]);
            }
            tuples newTuple(newValues);
            newRelation.addTuple(newTuple);
        }
        return newRelation;
    }

    relation rename(vector<string> newAttributes)
    {
        scheme newScheme(newAttributes);
        relation newRelation(id, newScheme);
        newRelation.setTuples(tuplesSet);
        return newRelation;
    }

    
    string toString() {
        stringstream out;
        for (auto tuple : tuplesSet)
        {
            for (unsigned int j = 0; j < schemes.getAttributes().size(); j++)
            {
                //cout << schemes.getAttributes().at(j) << '=' << tuple.at(j);
                if (j != 0) { out << ", "; }
                else { out << "  "; }
                out << schemes.getAttributes().at(j) << '=' << tuple.at(j);
            }
            //out << tuple.toString(schemes) << endl;
            if (schemes.getAttributes().size() > 0) { out << endl; }
        }
        return out.str();
    }
    string rString;
    /*
    for (auto i : DemTuples) {
        if (TheScheme.Size() != 0) {
            cout << "  ";
        }
        for (unsigned int j = 0; j < TheScheme.Size(); j++) {
            cout << TheScheme.At(j) + "=" + i.at(j);
            if (j < TheScheme.Size() - 1) {
                cout << ", ";
            }
        }
        if (TheScheme.Size() != 0) {
            cout << "\n";
        }
    }
    return rString;
    */
    
};
