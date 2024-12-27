defmodule Day3 do
  @filepath "./input.txt"

  defp get_input() do
    File.read!(@filepath)
  end

  defp chop_and_accumulate_while(str, _, acc) when str == "" do
    {str, acc}
  end

  defp chop_and_accumulate_while(str, _, acc) when str == nil do
    {str, acc}
  end

  defp chop_and_accumulate_while(str, _, acc) when length(str) == 0 do
    {str, acc}
  end

  defp chop_and_accumulate_while(str, condition, acc) do
    if condition.(String.at(str, 0)) do
      chop_and_accumulate_while(String.slice(str, 1..-1//1), condition, acc <> String.at(str, 0))
    else
      {str, acc}
    end
  end

  defp chop_and_accumulate_while(str, condition) do
    chop_and_accumulate_while(str, condition, "")
  end

  defp parse_number(num) do
    case Integer.parse(num) do
      :error -> 0
      {num, ""} -> num
      _ -> 0
    end
  end

  defp parse_numbers_for_multiplication(num1, num2) do
    {parse_number(num1), parse_number(num2)}
  end

  defp evaluate_equation(equation) do
    equation = String.reverse(equation)
    {rest_of_equation, num1} = chop_and_accumulate_while(equation, &(&1 != ","))
    rest_of_equation = String.slice(rest_of_equation, 1..-1//1)
    {rest_of_equation, num2} = chop_and_accumulate_while(rest_of_equation, &(&1 != "("))

    if String.starts_with?(rest_of_equation, "(lum") do
      {num1, num2} = parse_numbers_for_multiplication(String.reverse(num1), String.reverse(num2))
      num1 * num2
    else
      0
    end
  end

  defp get_multiplications_sum(items, acc) do
    if length(items) == 0 do
      acc
    else
      equation = hd(items)
      result = evaluate_equation(equation)
      get_multiplications_sum(tl(items), acc + result)
    end
  end

  defp get_multiplications_sum(items) do
    get_multiplications_sum(items, 0)
  end

  def part1() do
    multiplications =
      String.split(get_input(), ")", [:trim])
      |> Stream.filter(fn split -> String.length(String.trim(split)) > 0 end)
      |> Enum.to_list()
    get_multiplications_sum(multiplications)
  end

  def part2() do
    only_enabled_multiplications =
      String.split(get_input(), ~r{do\(\)})
      |> Stream.flat_map(fn muls  -> [hd(String.split(muls, ~r{don\'t\(\)}))] end)
      |> Stream.flat_map(fn line -> String.split(line, ")", [:trim]) end)
      |> Stream.filter(fn split -> String.length(String.trim(split)) > 0 end)
      |> Enum.to_list()
    get_multiplications_sum(only_enabled_multiplications)
  end
end

IO.puts("part1 answer: #{Day3.part1()}")
IO.puts("part2 answer: #{Day3.part2()}")
