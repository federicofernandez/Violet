#ifndef STATEFUL_TEST_H
#define STATEFUL_TEST_H

namespace Violet
{
	template <typename ResultType, typename State>
	class StatefulTest
	{
	public:

		typedef ResultType (*Predicate)(State &);

		StatefulTest(const char * name, const ResultType & desired, Predicate predicate) :
			m_name(name),
			m_desired(desired),
			m_predicate(predicate)
		{
		}

        StatefulTest(StatefulTest && rhs) :
            m_name(rhs.m_name),
            m_desired(std::move(rhs.m_desired)),
            m_predicate(std::move(rhs.m_predicate))
        {
        }

		template <typename TestEvaluator>
		bool evaluate(TestEvaluator & evaluator, State & state) const 
		{
			ResultType const actual = m_predicate(state);
			return evaluator.evaluate(m_name, m_desired, actual);
		}

    private:

		StatefulTest(const StatefulTest &) = delete;

	private:

		const char * m_name;
		ResultType m_desired;
		Predicate m_predicate;
	};
}

#endif