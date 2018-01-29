/*
Copyright (c) 2018 Patrick Demian

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <iostream>

namespace Details {
	//a constexpr string implementation
	template<int n>
	struct ConstexprString {
		int length;
		char str[n + 1];

		constexpr explicit ConstexprString() : length(n), str{} {}
		constexpr explicit ConstexprString(char const (&s)[n + 1]) : length(n), str{} {
			for (int i = 0; i < n; i++) {
				str[i] = s[i];
			}
		}
	};

	template<int n>
	constexpr auto GetConstexprString(char const (&str)[n]) {
		ConstexprString<n - 1> retval{};
		for (int i = 0; i < n; ++i) {
			retval.str[i] = str[i];
		}
		return retval;
	}

	//concat strings during compile time
	template<int n, int m>
	constexpr auto operator+(ConstexprString<n> const& x, ConstexprString<m> const& y) {
		ConstexprString<n + m> retval{};

		for (int i = 0; i < x.length; i++) {
			retval.str[i] = x.str[i];
		}
		for (int i = 0; i < y.length; i++) {
			retval.str[i + x.length] = y.str[i];
		}

		retval.str[n + m] = '\0';

		return retval;
	}

	//We need to convert the integer value to a string value for non-fizz/buzz values
	template<int n>
	struct ConstexprIota {
	private:
		//count the number of digits for optimal number of bytes used
		//rather than something like std::numeric_limits<int>::digits10
		//DigitCounter<12345> ==> 5
		static constexpr int DigitCounter() {
			int count = 0;
			for (int i = n; i > 0; i /= 10) {
				count++;
			}

			return count;
		}
	public:
		static constexpr auto Value() {
			int digit_count = DigitCounter();

			//unfortunately ConstexprString<digit_count + 1> doesn't work, we need to call it directly
			ConstexprString<DigitCounter() + 1> retval{};

			for (int i = digit_count - 1, number = n; i >= 0; i--, number /= 10) {

				//If you assume ASCII, you can subtract '0' from i
				//However this is more portable
				retval.str[i] = "0123456789"[number % 10];
			}

			retval.str[digit_count] = '\n';
			retval.str[digit_count + 1] = '\0';
			retval.length = digit_count + 1;
			return retval;
		}
	};

	template<int n, bool div_3, bool div_5>
	struct FizzBuzzValue { static constexpr auto Value = ConstexprIota<n>::Value(); };

	template<int n>
	struct FizzBuzzValue<n, true, true> { static constexpr auto Value = GetConstexprString("FizzBuzz\n"); };

	template<int n>
	struct FizzBuzzValue<n, true, false> { static constexpr auto Value = GetConstexprString("Fizz\n"); };

	template<int n>
	struct FizzBuzzValue<n, false, true> { static constexpr auto Value = GetConstexprString("Buzz\n"); };

};

template<int n>
struct FizzBuzz {
protected:
	static constexpr bool is_div3 = n % 3 == 0;
	static constexpr bool is_div5 = n % 5 == 0;
	static constexpr auto line = Details::FizzBuzzValue<n, is_div3, is_div5>::Value;
public:
	static constexpr auto string = FizzBuzz<n - 1>::string + line;
	static constexpr auto Value = string.str;

};

template<>
struct FizzBuzz<0> { 
	static constexpr auto string = Details::ConstexprString<0>();
	static constexpr auto Value = string.str;
};

int main() {
	puts(FizzBuzz<100>::Value);
	return 0;
}