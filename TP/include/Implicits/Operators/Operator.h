#pragma once

#include <vector>
#include <Vroom/Core/Assert.h>

#include "Implicits/implicits.h"

template <int MIN, int MAX>
class Operator : public AnalyticScalarField
{
public:
    Operator() = delete;
    
    template <typename... Fields>
    Operator(Fields&&... fields)
        : m_Fields{ std::forward<Fields>(fields)... }
    {}

    inline static consteval int GetMinChildrenCount() { return MIN; }

    inline static consteval int GetMaxChildrenCount() { return MAX; }

    virtual float Value(const glm::vec3& p) const override = 0;

    inline void setField(size_t f, const AnalyticScalarField* asf) override
    {
        VRM_ASSERT_MSG(f < m_Fields.size(), "Requested to edit field {} but the operator only holds {}.", f, m_Fields.size());
        m_Fields.at(f) = asf;
    }

    void addField(const AnalyticScalarField* asf) override
    {
        m_Fields.push_back(asf);
    }

    void removeField(size_t field) override
    {
        m_Fields.erase(std::next(m_Fields.begin(), field));
    }

    inline size_t getFieldsCount() const { return m_Fields.size(); }

protected:
    inline float fieldValue(size_t f, const glm::vec3& p) const
    {
        return m_Fields[f]->Value(p);
    }

private:
    std::vector<const AnalyticScalarField*> m_Fields;
};

template <int MIN>
using ArbitraryChildrenCountOperator = Operator<MIN, std::numeric_limits<int>::max()>;