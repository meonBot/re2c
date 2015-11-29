#ifndef _RE2C_CODEGEN_SKELETON_PATH_
#define _RE2C_CODEGEN_SKELETON_PATH_

#include <vector>

#include "src/ir/rule_rank.h"
#include "src/util/c99_stdint.h"

namespace re2c
{

struct rule_t
{
	rule_rank_t rank;
	bool restorectx;

	rule_t (rule_rank_t r, bool c)
		: rank (r)
		, restorectx (c)
	{}

	// needed by STL containers
	// same as 'std::pair' comparator
	bool operator < (const rule_t & r) const
	{
		return rank < r.rank
			|| (!(r.rank < rank) && restorectx < r.restorectx);
	}
};

template <typename arc_t>
class generic_path_t
{
	std::vector<arc_t> arcs;

	rule_t rule;
	size_t rule_pos;

	bool ctx;
	size_t ctx_pos;

public:
	explicit generic_path_t (rule_t r, bool c)
		: arcs ()
		, rule (r)
		, rule_pos (0)
		, ctx (c)
		, ctx_pos (0)
	{}
	size_t len () const
	{
		return arcs.size ();
	}
	size_t len_matching () const
	{
		return rule.restorectx
			? ctx_pos
			: rule_pos;
	}
	rule_rank_t match () const
	{
		return rule.rank;
	}
	const arc_t & operator [] (size_t i) const
	{
		return arcs[i];
	}
	void extend (rule_t r, bool c, const arc_t & a)
	{
		arcs.push_back (a);
		if (!r.rank.is_none ())
		{
			rule = r;
			rule_pos = arcs.size ();
		}
		if (c)
		{
			ctx = true;
			ctx_pos = arcs.size ();
		}
	}
	void append (const arc_t & a, const generic_path_t<arc_t> * p)
	{
		arcs.push_back (a);
		if (!p->rule.rank.is_none ())
		{
			rule = p->rule;
			rule_pos = arcs.size () + p->rule_pos;
		}
		if (p->ctx)
		{
			ctx = true;
			ctx_pos = arcs.size () + p->ctx_pos;
		}
		arcs.insert (arcs.end (), p->arcs.begin (), p->arcs.end ());
	}

	template <typename arc1_t, typename arc2_t>
		friend size_t len_matching (const generic_path_t<arc1_t> & prefix, const generic_path_t<arc2_t> & suffix);
	template <typename arc1_t, typename arc2_t>
		friend rule_rank_t match (const generic_path_t<arc1_t> & prefix, const generic_path_t<arc2_t> & suffix);
};

template <typename arc1_t, typename arc2_t>
	size_t len_matching (const generic_path_t<arc1_t> & prefix, const generic_path_t<arc2_t> & suffix)
{
	const bool none = suffix.rule.rank.is_none ();
	bool restorectx = none
		? prefix.rule.restorectx
		: suffix.rule.restorectx;
	const size_t rule_pos = none
		? prefix.rule_pos
		: prefix.arcs.size () + suffix.rule_pos;

	const size_t ctx_pos = suffix.ctx
		? prefix.arcs.size () + suffix.ctx_pos
		: prefix.ctx_pos;

	return restorectx
		? ctx_pos
		: rule_pos;
}

template <typename arc1_t, typename arc2_t>
	rule_rank_t match (const generic_path_t<arc1_t> & prefix, const generic_path_t<arc2_t> & suffix)
{
	return suffix.rule.rank.is_none ()
		? prefix.rule.rank
		: suffix.rule.rank;
}

typedef generic_path_t<uint32_t> path_t;

typedef std::vector<uint32_t> multiarc_t;
typedef generic_path_t<const multiarc_t *> multipath_t;

} // namespace re2c

#endif // _RE2C_CODEGEN_SKELETON_PATH_