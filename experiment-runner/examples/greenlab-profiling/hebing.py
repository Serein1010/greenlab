import pandas as pd


def merge_tables(file_list):
    merged_df = pd.read_csv(file_list[0])
    for file in file_list[1:]:
        df = pd.read_csv(file)
        merged_df = pd.concat([merged_df, df])
    merged_df = merged_df.sort_values(by=["Algorithm", "Language", "GPT"])
    return merged_df


files = [
    "merged_result0.csv",
    "merged_result1.csv",
    "merged_result2.csv",
    "merged_result3.csv",
    "merged_result4.csv",
    "merged_result5.csv",
]

merged_df = merge_tables(files)
merged_df["__run_id"] = ["run_" + str(i) for i in range(1, len(merged_df) + 1)]
merged_to_drop_column = "Repetitions"
merged_df = merged_df.drop(columns=merged_to_drop_column)
merged_df.to_csv("merged.csv", index=False)
