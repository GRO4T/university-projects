import pandas as pd


def preprocess_reimbursed_dataset(
    reimbursed_medicines_path: str, sheet: str, reimbursed_cols: list
) -> pd.DataFrame:
    df = pd.read_excel(
        reimbursed_medicines_path,
        sheet_name=sheet,
        skiprows=1,
        usecols=reimbursed_cols,
        decimal=",",
    )
    df = df.rename(
        columns={
            "Zawartość opakowania": "Package",
            "Numer GTIN lub inny kod jednoznacznie identyfikujący produkt": "GTIN",
            "Cena detaliczna": "Price",
            "Wysokość dopłaty świadczeniobiorcy": "Supplement",
        }
    )
    df["Price"] = df["Price"].astype(float)
    df["Supplement"] = df["Supplement"].astype(float)
    df["Refund_amount"] = df["Price"] - df["Supplement"]
    df["Package"] = df["Package"].str.extract(r"(\d+)").astype(int)
    df.drop(["Price", "Supplement"], axis=1, inplace=True)

    return df


def preprocess_rpl_dataset(rpl_path: str, rpl_cols: list) -> pd.DataFrame:
    df = pd.read_csv(rpl_path, sep=";", usecols=rpl_cols)
    df.dropna(subset=["Substancja czynna", "Podmiot odpowiedzialny"], inplace=True)
    df["Opakowanie"] = df["Opakowanie"].str.replace("\n", " ")
    df["GTIN"] = (
        df["Opakowanie"]
        .str.extractall(r"(\d{12,14}|\d{8})")
        .groupby(level=0)[0]
        .apply(list)
    )
    df = df.explode("GTIN")
    df.drop("Opakowanie", axis=1, inplace=True)
    df.dropna(subset=["GTIN"], inplace=True)
    df["GTIN"] = df["GTIN"].astype(int)

    return df
