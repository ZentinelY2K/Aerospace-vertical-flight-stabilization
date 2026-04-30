# ====== 1. IMPORTS ======
import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
from sklearn.tree import DecisionTreeClassifier
from sklearn.metrics import accuracy_score

# ====== 2. LOAD DATA ======
# Replace this with your own file later
data = pd.read_csv("/home/zentinely2k/Documents/Y2K-Zone/AI_Learning_Journey/odd_or_even.csv")

# ====== 3. SELECT FEATURES & LABEL ======
# Features = inputs
X = data[["feature1"]]

# Labels = what you want to predict
y = data["label"]

# ====== 4. SPLIT DATA ======
X_train, X_test, y_train, y_test = train_test_split(
    X, y, test_size=0.2, random_state=42
)

# ====== 5. SCALE DATA ======
scaler = StandardScaler()
X_train = scaler.fit_transform(X_train)
X_test = scaler.transform(X_test)

# ====== 6. CREATE MODEL ======
model = DecisionTreeClassifier()

# ====== 7. TRAIN ======
model.fit(X_train, y_train)

# ====== 8. PREDICT ======
y_pred = model.predict(X_test)

# ====== 9. EVALUATE ======
accuracy = accuracy_score(y_test, y_pred)
print("Accuracy:", accuracy)

# ====== 10. TEST WITH YOUR OWN INPUT ======
# Example: change these values
new_data = [[51]]

new_data = scaler.transform(new_data)
prediction = model.predict(new_data)

print("Prediction:", prediction)