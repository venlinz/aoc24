defmodule Solution do
  @file_path "./input.txt"

  defp get_stream() do
    File.stream!(@file_path)
    |> Stream.map(&String.trim/1)
    |> Stream.flat_map(&String.split/1)
    |> Stream.map(&String.trim/1)
  end

  defp get_list() do
    get_stream()
    |> Enum.map(&Integer.parse/1)
    |> Enum.map(fn x -> elem(x, 0) end)
    |> Enum.to_list()
  end

  defp get_left_list() do
    get_list() |> Stream.take_every(2) |> Enum.sort()
  end

  defp get_right_list() do
    get_list() |> Stream.drop_every(2) |> Enum.sort()
  end

  def part1() do
    Enum.zip_reduce(get_left_list(), get_right_list(), 0, fn x, y, acc -> acc + abs(x - y) end)
  end

  def part2() do
    left_elements_map = get_left_list() |> Enum.sort() |> Enum.dedup()
    sorted_right_list = get_right_list() |> Enum.sort()
    frequency_map =
      left_elements_map
      |> Enum.map(fn x -> {x, Enum.count(sorted_right_list, fn y -> x == y end)} end)
      |> Map.new()

    get_left_list() |> Enum.reduce(0, fn x, acc -> acc + frequency_map[x] * x end)
  end
end

IO.puts("part1 answer: #{Solution.part1()}")
IO.puts("part2 answer: #{Solution.part2()}")
