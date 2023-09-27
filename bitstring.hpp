#include <vector>
#include <numeric>
#include <span>
#include <algorithm>
#include <cstddef>
#include <iterator>
#include <iostream>
#include <cassert>
#include <cmath>

extern std::mt19937 &get_generator();

class bitstring : public std::vector<uint8_t>
{
private:
    // The minimum and maximum values that can be represented by the bitstring.
    double min, max;
    // The number of groups (variables) in the bitstring.
    size_t groups;

    /**
     * @brief Assert that the bitstring is valid.
     * A bitstring is valid if:
     * 1. The size of the bitstring is divisible by the number of groups.
     * 2. The minimum value is less than the maximum value.
     * 3. The number of groups is greater than 0.
     */
    void assertions() const
    {
        assert(this->size() % this->groups == 0);
        assert(this->min < this->max);
        assert(this->groups > 0);
        assert(this->size() / this->groups <= 64);
    };

public:
    // Constructors and destructors.
    bitstring(std::vector<uint8_t> in_vector, double in_min, double in_max, size_t in_groups) : std::vector<uint8_t>(in_vector),
                                                                                                min(in_min),
                                                                                                max(in_max),
                                                                                                groups(in_groups)
    {
        assertions();
    };
    bitstring(size_t size_of_one_group, double in_min, double in_max, size_t in_groups) : std::vector<uint8_t>(size_of_one_group * in_groups),
                                                                             min(in_min),
                                                                             max(in_max),
                                                                             groups(in_groups)
    {
        assertions();
    };

    ~bitstring() = default;

    bitstring(const bitstring &other) : std::vector<uint8_t>(other),
                                        min(other.min),
                                        max(other.max),
                                        groups(other.groups)
    {
        assertions();
    };
    bitstring(bitstring &&other) : std::vector<uint8_t>(std::move(other)),
                                   min(other.min),
                                   max(other.max),
                                   groups(other.groups)
    {
        assertions();
    };

    bitstring &operator=(const bitstring &other)
    {
        this->assign(other.begin(), other.end());
        this->min = other.min;
        this->max = other.max;
        this->groups = other.groups;
        assertions();
        return *this;
    };
    bitstring &operator=(bitstring &&other)
    {
        this->assign(other.begin(), other.end());
        this->min = other.min;
        this->max = other.max;
        this->groups = other.groups;
        assertions();
        return *this;
    };

    // Overload the equality operator.
    bool operator==(const bitstring &other) const
    {
        return this->min == other.min && this->max == other.max && this->groups == other.groups && std::equal(this->begin(), this->end(), other.begin());
    };

    // Overload the output operator.
    friend std::ostream &operator<<(std::ostream &out, const bitstring &c)
    {
        for (auto val : c)
        {
            out << std::to_string(val);
        }
        return out;
    };

    /**
     * @brief Randomize the bitstring.
     * Randomize the bitstring by setting each bit to a random value.
     */
    void randomize()
    {
        std::uniform_int_distribution<uint8_t> distribution(0, 1);
        for (size_t i = 0; i < this->size(); i++)
        {
            this->at(i) = distribution(get_generator());
        }
    };

    /**
     * @brief Decode the bitstring.
     * Decode the bitstring by converting each group of bits to a double.
     * @return The decoded bitstring as a vector of doubles.
     */
    std::vector<double> decode() const
    {
        std::vector<double> result;
        assert(this->size() % this->groups == 0);
        assert (this->size() > 0);
        assert (this->groups > 0);
        auto group_size = this->size() / this->groups;
        for (size_t i = 0; i < this->size(); i += group_size)
        {
            result.push_back(this->decode(i, i + group_size - 1));
        }
        return result;
    };

    /**
     * @brief Decode the bitstring.
     * Decode the bitstring by converting the bits in the given range to a double.
     * @param start The start index of the range.
     * @param end The end index of the range.
     * @return The decoded bitstring as a double.
     */
    double decode(size_t start, size_t end) const
    {
        // First convert the binary number to an unsigned long long.
        assert(end < this->size());
        assert(start < end);
        assert(end - start == this->size() / this->groups - 1);
        uint32_t val = 0;
        for (size_t i = start; i <= end; i++)
        {
            val *= 2;
            val += this->at(i);
        }
        // Then convert the unsigned long long to a double.
        auto divided = (double) val / max_full_size();
        assert(divided >= 0.0 && divided <= 1.0);
        auto range = max - min;
        auto res = min + range * divided;
        assert(res >= min && res <= max);
        return res;
    };

    /**
     * @brief Encode the bitstring.
     * Encode the bitstring by converting the given vector of doubles to a binary number.
     * @param val The vector of doubles to encode.
     * @return The encoded bitstring.
     */
    void encode(std::span<double> val)
    {
        // Check if the given vector is valid.
        assert(val.size() == this->groups);
        for (auto v : val)
        {
            assert(v >= min && v <= max);
        }
        // Encode the vector.
        this->resize(val.size());
        for (auto v : val)
        {
            // First convert the double to an unsigned long long.
            auto int_val = (uint64_t)((v - min) * max_full_size() / (max - min));
            // Then convert the unsigned long long to a binary number.
            for (size_t i = 0; i < this->size(); i++)
            {
                this->at(this->size() - i - 1) = int_val & 1;
                int_val /= 2;
            }
        }
    };

    /**
     * @brief Flip the bit at the given index.
     * @param index The index of the bit to flip.
     */
    void flip(size_t index)
    {
        this->at(index) ^= 1;
    };

    /**
     * @brief Print the bitstring.
     */
    void print() const
    {
        std::cout << *this << std::endl;
    };

    /**
     * @brief Get the number of values each variable in the bitstring can have.
     * @return The number of values each variable in the bitstring can have.
     */
    double max_full_size() const
    {
        return std::pow(2, this->size() / this->groups) - 1;
    };

    /**
     * @brief Get the minimum value that can be represented by the bitstring.
     * @return The minimum value that can be represented by the bitstring.
     */
    const double &get_min() const
    {
        return this->min;
    };

    /**
     * @brief Get the maximum value that can be represented by the bitstring.
     * @return The maximum value that can be represented by the bitstring.
     */
    const double &get_max() const
    {
        return this->max;
    };

    /**
     * @brief Get the number of groups (variables) in the bitstring.
     * @return The number of groups (variables) in the bitstring.
     */
    const size_t &get_groups() const
    {
        return this->groups;
    };
};