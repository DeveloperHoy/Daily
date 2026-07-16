<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>MyApp 로그인</title>
</head>
<body>
    <h1>로그인</h1>
    <form action="login" method="post">
        이름: <input type="text" name="username" required>
        <button type="submit">로그인</button>
    </form>

    <hr>
    <p>다른 예제:</p>
    <ul>
        <li><a href="hello?name=테스트">HelloServlet (GET 처리)</a></li>
    </ul>
</body>
</html>
