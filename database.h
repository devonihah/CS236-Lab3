#pragma once
#include <string>
#include <vector>
#include <map>
#include "relation.h"
#include "tuple.h"
using namespace std;

class database {
private:
	map<string, relation> relations;
public:
	database() = default;
	void addRelation(relation newRelation) { relations[newRelation.getID()] = newRelation; }
	void addTupleToRelation(string id, tuples newTuple) { relations[id].addTuple(newTuple); }
	relation findRelation(string relationName) { return relations[relationName]; }
};