defmodule Day2 do
  @file_path "./input.txt"

  defp safe_lvls_counter_with_one_lvl_tolerance(levels, total_safe_lvls_count) do
    expected_levels_count = Enum.count(levels)
    safe_levels_count = get_safe_levels_count(levels)

    if safe_levels_count == expected_levels_count do
      total_safe_lvls_count + 1
    else
      unsafe_levels = get_unsafe_levels(levels)
      expected_levels_count_with_tolerance = expected_levels_count - 2
      can_create_safe_lvl_after_removing_a_lvl =
        unsafe_levels
        |> Stream.map(fn unsafe_lvl -> elem(unsafe_lvl, 1) end)
        |> Enum.sort()
        |> Enum.dedup()
        |> Stream.map(fn unsafe_lvl_index ->
          elem(List.pop_at(Enum.to_list(levels), unsafe_lvl_index), 1)
        end)
        |> Stream.map(fn new_lvls -> get_safe_levels(new_lvls) end)
        |> Enum.any?(fn new_safe_lvls ->
          Enum.count(new_safe_lvls) == expected_levels_count_with_tolerance
        end)

      if can_create_safe_lvl_after_removing_a_lvl do
        total_safe_lvls_count + 1
      else
        total_safe_lvls_count
      end
    end
  end

  defp safe_lvls_counter(levels, total_safe_lvls_count) do
    safe_levels_count = get_safe_levels_count(levels)
    expected_levels_count = Enum.count(levels)

    if expected_levels_count == safe_levels_count do
      total_safe_lvls_count + 1
    else
      total_safe_lvls_count
    end
  end

  defp solve(answer_function) when is_function(answer_function) do
    answer =
      get_levels_str_list()
      |> Enum.map(&get_levels/1)
      |> Enum.reduce(0, answer_function)

    answer
  end

  def part1() do
    solve(&safe_lvls_counter/2)
  end

  def part2() do
    solve(&safe_lvls_counter_with_one_lvl_tolerance/2)
  end

  defp get_levels_str_list() do
    File.stream!(@file_path)
    |> Stream.map(&String.trim/1)
  end

  defp get_levels(report) do
    String.split(report)
    |> Stream.map(&String.trim/1)
    |> Stream.map(&Integer.parse/1)
    |> Stream.map(fn x -> elem(x, 0) end)
  end

  defp signal_direction(level1, level2) do
    if level1 < level2 do
      :increase
    else
      :decrease
    end
  end

  defp signal_direction(adj_levels) when is_tuple(adj_levels) do
    level1 = elem(adj_levels, 0)
    level2 = elem(adj_levels, 1)

    signal_direction(level1, level2)
  end

  defp signal_direction(levels) do
    level1 = levels |> Enum.take(1)
    level2 = levels |> Stream.drop(1) |> Enum.take(1)

    signal_direction(level1, level2)
  end

  defp get_safe_levels_count(levels) do
    (get_safe_levels_stream(levels) |> Enum.count()) + 1
  end

  defp get_diff(adj_lvls_zip) do
    abs(elem(adj_lvls_zip, 0) - elem(adj_lvls_zip, 1))
  end

  defp get_safe_levels_stream(levels) do
    signal_direction = signal_direction(levels)

    levels
    |> Stream.zip(levels |> Stream.drop(1))
    |> Stream.filter(fn adj_lvls ->
      diff = get_diff(adj_lvls)
      diff >= 1 && diff <= 3
    end)
    |> Stream.filter(fn adj_lvls -> signal_direction(adj_lvls) == signal_direction end)
  end

  defp get_safe_levels(levels) do
    get_safe_levels_stream(levels) |> Enum.to_list()
  end

  defp get_unsafe_levels(levels) do
    signal_direction = signal_direction(levels)

    unsafe_levels_by_diff =
      levels
      |> Stream.with_index()
      |> Stream.zip(levels |> Stream.with_index() |> Stream.drop(1))
      |> Stream.map(fn adj_lvls -> [elem(adj_lvls, 0), elem(adj_lvls, 1)] end)
      |> Stream.filter(fn adj_lvls ->
        diff = get_diff({elem(hd(adj_lvls), 0), elem(hd(adj_lvls), 1)})
        !(diff >= 1 && diff <= 3)
      end)
      |> Stream.flat_map(fn x -> x end)
      |> Enum.to_list()

    unsafe_levels_by_change_in_direction =
      levels
      |> Stream.with_index()
      |> Stream.zip(levels |> Stream.with_index() |> Stream.drop(1))
      |> Stream.map(fn adj_lvls -> [elem(adj_lvls, 0), elem(adj_lvls, 1)] end)
      |> Stream.filter(fn adj_lvls_index ->
        adj_lvls = {elem(hd(adj_lvls_index), 0), elem(hd(tl(adj_lvls_index)), 0)}
        signal_direction(adj_lvls) != signal_direction
      end)
      |> Stream.flat_map(fn x -> x end)
      |> Enum.to_list()

    need_to_include_initial_elems =
      unsafe_levels_by_change_in_direction
      |> Enum.any?(fn unsafe_lvl_with_idx -> elem(unsafe_lvl_with_idx, 1) <= 2 end)

    unsafe_levels_by_change_in_direction =
      if need_to_include_initial_elems do
        (levels
         |> Stream.with_index()
         |> Stream.take(2)
         |> Enum.to_list()) ++
          unsafe_levels_by_change_in_direction
      else
        unsafe_levels_by_change_in_direction
      end

    unsafe_levels_by_diff ++ unsafe_levels_by_change_in_direction
  end
end

IO.puts("part1 answer: #{Day2.part1()}")
IO.puts("part2 answer: #{Day2.part2()}")
